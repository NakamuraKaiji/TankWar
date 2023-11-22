#include "pch.h"
#include "EnemyTank.h"
#include "Utilities/Resources.h"
#include <random>

using namespace DirectX;


// コンストラクタ
EnemyTank::EnemyTank(int x, int y)
	: GameObject(static_cast<int>(ObjectID::Enemy), x, y, ENEMY_RADIUS, ENEMY_FRICTION, PLAYER_WEIGHT)
	, m_bodyModel{}
	, m_turretModel{}
	, m_tankPos{}
	, m_bodyRotate{}
	, m_turretRotate{}
	, m_coolTime(0.0f)
	, m_velocity{}
	, m_mass(1.0f)
	, m_maxSpeed(1.0f)
	, m_maxForce(1.0f)
	, m_wanderOrientation(0.0f)
{
}

// 初期化
void EnemyTank::Initialize()
{
	// 車体モデルを取得
	m_bodyModel = Resources::GetInstance()->GetEnemyTankBody();
	// 砲身モデルを取得
	m_turretModel = Resources::GetInstance()->GetEnemyTankTurret();

	// 各パーツの作成
	m_parts[ROOT]   = std::make_unique<Imase::ModelPart>();
	m_parts[BODY]   = std::make_unique<Imase::ModelPart>(m_bodyModel.get());
	m_parts[TURRET] = std::make_unique<Imase::ModelPart>(m_turretModel.get());
	// 各パーツを連結する
	// ROOTから車体の親子関係
	m_parts[ROOT]->SetChild(m_parts[BODY].get());
	// 車体から砲身の親子関係
	m_parts[BODY]->SetChild(m_parts[TURRET].get());

	m_wanderOrientation = 0.0f;

	m_tankPos = SimpleMath::Vector3(0.0f, 0.0f, 5.0f);

}

// 更新
bool EnemyTank::Update(float elapsedTime)
{
	// 操舵力の定義
	SimpleMath::Vector3 steeringforce = SimpleMath::Vector3::Zero;

	// 探索行動
	//steeringforce += Seek(m_playerPos);

	// 徘徊行動
	steeringforce += Wander(&m_wanderOrientation, 1.0f, 2.0f, 0.2f, elapsedTime);

	float radius = 7.0f;
	if (m_tankPos.Length() > radius)
	{
		steeringforce += Seek(SimpleMath::Vector3::Zero);
	}

	// 物体に与えられた力[m/s2]
	SimpleMath::Vector3 force = steeringforce / elapsedTime;

	// 並進運動の実行
	// 力の調整
	force = Truncate(force, m_maxForce);

	// 加速度の算出(加速度 = 力 / 質量)
	SimpleMath::Vector3 acceleration = force / m_mass;

	// 速度の更新および調整
	m_velocity += acceleration * elapsedTime;
	m_velocity = Truncate(m_velocity, m_maxSpeed);

	// 座標の更新
	m_tankPos += m_velocity * elapsedTime;

	// 進行方向を向く
	TurnHeading(m_velocity);

	// 敵からプレイヤーに向かうベクトルを計算する
	SimpleMath::Vector3 toTarget = /*GetTarget()->GetPosition()*/ - m_tankPos;

	// 回転角を計算する
	m_turretRotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, atan2(toTarget.x, toTarget.z));
	
	// 砲弾の発射間隔
	m_coolTime += elapsedTime;
	// スペースキーで発射
	if (m_coolTime >= 2.0f)
	{
		//PlayScene::Shot(m_tankPos + SimpleMath::Vector3::Transform(
		//	SimpleMath::Vector3(0.0f, 0.0f, 1.5f), SimpleMath::Matrix::CreateFromQuaternion(m_bodyRotate * m_turretRotate)),
		//	m_bodyRotate * m_turretRotate
		//);
		m_coolTime = 0.0f;
	}

	// 基底クラスの更新関数を呼び出して移動する
	GameObject::Update(elapsedTime);

	// 速さを制限する
	LimitSpeed(ENEMY_MAX_SPEED);


	// 衝突判定マネージャーに登録
	pCollisionManager.AddObject(this);

	return true;
}

// 描画
void EnemyTank::Render()
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_graphics->GetCommonStates();

	// ビュー行列と射影行列を取得
	SimpleMath::Matrix view, proj;
	view = m_graphics->GetViewMatrix();
	proj = m_graphics->GetProjectionMatrix();

	// 戦車の回転
	m_parts[BODY]->SetTransformMatrix(SimpleMath::Matrix::CreateFromQuaternion(m_bodyRotate));
	// 砲身の回転
	m_parts[TURRET]->SetTransformMatrix(SimpleMath::Matrix::CreateFromQuaternion(m_turretRotate));
	// 戦車の移動
	SimpleMath::Matrix mat = SimpleMath::Matrix::CreateFromQuaternion(m_bodyRotate) * SimpleMath::Matrix::CreateTranslation(m_tankPos);
	m_parts[BODY]->SetTransformMatrix(mat);

	// 戦車の描画
	//SimpleMath::Matrix world;
	//world *= SimpleMath::Matrix::CreateScale(0.01f, 0.01f, 0.01f);
	m_parts[ROOT]->UpdateMatrix();
	m_parts[ROOT]->Draw(context, *states, view, proj);

}

void EnemyTank::OnHit(GameObject* object)
{
}

void EnemyTank::Hit()
{
}

void EnemyTank::OnHit_Bullet(GameObject* object)
{
}


// 終了
void EnemyTank::Finalize()
{
}

// 指定の方向に向く
void EnemyTank::TurnHeading(const DirectX::SimpleMath::Vector3& direction)
{
	if (direction.Length() > 0.0001f)
	{
		SimpleMath::Quaternion rotation = SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::Forward, direction);
		m_bodyRotate = rotation;
	}
}

// 探索行動
DirectX::SimpleMath::Vector3 EnemyTank::Seek(const DirectX::SimpleMath::Vector3& targetPos)
{
	// 目標位置への方向ベクトルの算出
	SimpleMath::Vector3 toTarget = targetPos - m_tankPos;

	// 期待速度の算出
	toTarget.Normalize();
	SimpleMath::Vector3 desiredVelocity = toTarget * m_maxSpeed;

	// 操舵力の算出
	SimpleMath::Vector3 steeringForce = desiredVelocity - m_velocity;

	return steeringForce;
}

// 徘徊行動
DirectX::SimpleMath::Vector3 EnemyTank::Wander(float* wanderOriantation, float wanderRadius, float wanderDistance, float wanderRotate, float time)
{
	static std::random_device                    seedGenerator;
	static std::mt19937                          randomGenerator{ seedGenerator() };
	static std::uniform_real_distribution<float> distribution{ -1.0, 1.0 };

	// ターゲットへの角度を変化
	*wanderOriantation += (XM_PI * wanderRotate) * distribution(randomGenerator);

	// 徘徊行動用円の円周上のターゲットの位置の演出
	SimpleMath::Vector3 targetOriantation = SimpleMath::Vector3{ std::cos(*wanderOriantation), 0.0f, std::sin(*wanderOriantation) };
	SimpleMath::Vector3 toTarget = targetOriantation * wanderRadius;

	// 徘徊行動用の中心の算出
	SimpleMath::Vector3 wanderCenter = m_tankPos + SimpleMath::Vector3::Transform(SimpleMath::Vector3::Forward, m_bodyRotate);

	// ターゲットの位置の算出(ワールド座標)
	SimpleMath::Vector3 targetPosition = wanderCenter + toTarget;

	// 探索行動による操舵力の算出
	SimpleMath::Vector3 steeringForce = Seek(targetPosition);

	return steeringForce;
}

// ベクトルの長さの切り捨て
inline DirectX::SimpleMath::Vector3 EnemyTank::Truncate(const DirectX::SimpleMath::Vector3& v, float maxLength)
{
	const float maxLengthSquared = maxLength * maxLength;
	const float vecLengthSquared = v.LengthSquared();
	if (vecLengthSquared <= maxLengthSquared)
		return v;
	else
		return v * (maxLength / v.Length());
}


