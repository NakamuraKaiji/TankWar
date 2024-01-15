#include "pch.h"
#include "EnemyTank.h"
#include "Utilities/Resources.h"
#include "Bullet.h"
#include <random>

using namespace DirectX;

// コンストラクタ
EnemyTank::EnemyTank(const GameResources& gameResources, DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotate)
	: GameObject(static_cast<int>(ObjectID::Enemy), position, rotate, ENEMY_RADIUS, ENEMY_FRICTION, PLAYER_WEIGHT)
	, m_gameResources(gameResources)
	, m_bodyModel{}
	, m_turretModel{}
	, m_bodyRotate{}
	, m_turretRotate{}
	, m_interval(0.0f)
	, m_velocity{}
	, m_mass(1.0f)
	, m_wanderOrientation(0.0f)
	, m_target(nullptr)
	, m_enemyState(EnemyState::Normal)
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

	// 描画順の設定
	SetOt(static_cast<int>(OT_Priority::OT_Object));
}

// 更新
bool EnemyTank::Update(float elapsedTime)
{
	// 状態によって処理を分岐させる
	switch (m_enemyState)
	{
	case EnemyState::Normal:
		Normal(elapsedTime);
		break;
	case EnemyState::Hit:
		Hit();
		break;
	default:
		break;
	}

	// 基底クラスの更新関数を呼び出して移動する
	GameObject::Update(elapsedTime);

	// 速さを制限する
	LimitSpeed(ENEMY_MAX_SPEED);

	// 衝突判定マネージャーに登録
	m_gameResources.pCollisionManager->AddObject(this);

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
	SimpleMath::Matrix mat = SimpleMath::Matrix::CreateFromQuaternion(m_bodyRotate) 
						   * SimpleMath::Matrix::CreateTranslation(GetPosition());
	m_parts[BODY]->SetTransformMatrix(mat);

	// 戦車の描画
	m_parts[ROOT]->UpdateMatrix();
	m_parts[ROOT]->Draw(context, *states, view, proj);

}

// 衝突したら呼ばれる関数
void EnemyTank::OnHit(GameObject* object)
{
	// 停止させる
	SetVelocity(SimpleMath::Vector3::Zero);
	// 移動させる
	SimpleMath::Vector3 dir = GetPosition() - object->GetPosition();
	dir.y = 0.0f;
	dir.Normalize();
	AddForce(dir, object->GetHitForce());
	// 衝突状態へ
	m_enemyState = EnemyState::Hit;
}

// リセット
void EnemyTank::Reset()
{
}

// 通常時の関数
void EnemyTank::Normal(float elapsedTime)
{
	// 操舵力の定義
	SimpleMath::Vector3 steeringforce = SimpleMath::Vector3::Zero;
	// 探索行動
	//steeringforce += Seek(m_target->GetPosition());

	// 徘徊行動
	steeringforce += Wander(&m_wanderOrientation, 1.0f, 2.0f, 0.2f, elapsedTime);

	SimpleMath::Vector3 toTarget = GetPosition() - m_target->GetPosition();

	float radius = 5.0f;
	if (toTarget.Length() > radius)
	{
		steeringforce -= Seek(m_target->GetPosition());
	}

	// 物体に与えられた力[m/s2]
	SimpleMath::Vector3 force = steeringforce / elapsedTime;

	// 並進運動の実行
	// 力の調整
	force = Truncate(force, ENEMY_MOVE_FORCE);

	// 加速度の算出(加速度 = 力 / 質量)
	SimpleMath::Vector3 acceleration = force / m_mass;

	// 速度の更新および調整
	m_velocity += acceleration * elapsedTime;
	m_velocity = Truncate(m_velocity, ENEMY_MAX_SPEED);

	AddForce(SimpleMath::Vector3::Transform(-OBJECT_FORWARD, m_bodyRotate), ENEMY_MOVE_FORCE);

	// 座標の更新
	SetPosition(GetPosition() - (m_velocity * elapsedTime));

	// 進行方向を向く
	TurnHeading(m_velocity);

	// 回転角を計算する
	//m_turretRotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, atan2(toTarget.z, toTarget.x));

	// 砲弾の発射間隔
	m_interval += elapsedTime;
	// 2秒間隔で発射
	if (m_interval >= 2.0f)
	{
		// 弾を発射する
		Bullet* bulletTask = this->GetTaskManager()->AddTask<Bullet>(m_gameResources, GetPosition(), m_bodyRotate * m_turretRotate);
		// 親を変更する
		bulletTask->ChangeParent(this->GetTaskManager()->GetRootTask());

		SetVelocity(SimpleMath::Vector3::Zero);

		m_interval = 0.0f;
	}
}

// 衝突中関数
void EnemyTank::Hit()
{
	// 摩擦により停止したら
	if (GetVelocity() == SimpleMath::Vector3::Zero)
	{
		m_enemyState = EnemyState::Normal;
	}
}

// 指定の方向に向く
void EnemyTank::TurnHeading(const DirectX::SimpleMath::Vector3& direction)
{
	if (direction.Length() > 0.001f)
	{
		SimpleMath::Quaternion rotation = SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::Forward, direction);
		m_bodyRotate = rotation;
	}
}

// 探索行動
DirectX::SimpleMath::Vector3 EnemyTank::Seek(const DirectX::SimpleMath::Vector3& targetPos)
{
	// 目標位置への方向ベクトルの算出
	SimpleMath::Vector3 toTarget = targetPos - GetPosition();

	// 期待速度の算出
	toTarget.Normalize();
	SimpleMath::Vector3 desiredVelocity = toTarget * ENEMY_MAX_SPEED;

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
	SimpleMath::Vector3 wanderCenter = GetPosition() + SimpleMath::Vector3::Transform(SimpleMath::Vector3::Forward, m_bodyRotate);

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


