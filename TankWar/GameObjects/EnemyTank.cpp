#include "pch.h"
#include "EnemyTank.h"
#include "Utilities/Resources.h"
#include "Bullet.h"
#include "SmokeEffect.h"
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
	, m_bulletTime(0.0f)
	, m_velocity{}
	, m_mass(1.0f)
	, m_wanderOrientation(0.0f)
	, m_target(nullptr)
	, m_enemyState(EnemyState::Normal)
	, m_invincibleTime(0.0f)
	, m_smokeTime(0.0f)
	, m_shotFlag(false)
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
	m_parts[ROOT]   = std::make_unique<ModelPart>();
	m_parts[BODY]   = std::make_unique<ModelPart>(m_bodyModel.get());
	m_parts[TURRET] = std::make_unique<ModelPart>(m_turretModel.get());
	// 各パーツを連結する
	// ROOTから車体の親子関係
	m_parts[ROOT]->SetChild(m_parts[BODY].get());
	// 車体から砲身の親子関係
	m_parts[BODY]->SetChild(m_parts[TURRET].get());

	// 初期化
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

	m_invincibleTime += elapsedTime;

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


	// 砲身の回転
	m_parts[TURRET]->SetTransformMatrix(SimpleMath::Matrix::CreateFromQuaternion(m_turretRotate));
	// 戦車の移動と回転
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
	// 衝突した相手によって処理を変える
	switch (static_cast<ObjectID>(object->GetID()))
	{
	case ObjectID::Bullet:			// 砲弾
		OnHit_Bullet();
		break;
	case ObjectID::Player:
		OnHit_Player();
	default:
		break;
	}
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

	// 徘徊行動
	steeringforce += Wander(&m_wanderOrientation, 1.0f, 2.0f, 0.2f);

	// ターゲットまでの距離
	SimpleMath::Vector3 toTarget = m_target->GetPosition() - GetPosition();

	// 範囲外に出たら戻す
	if (toTarget.Length() > ENEMY_SEEK_RANGE)
	{
		steeringforce -= Seek(m_target->GetPosition());
	}
	// 近付きすぎたら離れる
	if (toTarget.Length() <= ENEMY_FLEE_RANGE)
	{
		steeringforce -= Flee(m_target->GetPosition());
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
	float targetRotation = std::atan2f(toTarget.x, toTarget.z);
	m_turretRotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, targetRotation);
	m_turretRotate = XMQuaternionMultiply(m_turretRotate, XMQuaternionInverse(m_bodyRotate));

	// 砲弾の発射間隔
	m_bulletTime += elapsedTime;
	m_shotFlag = false;
	// 2秒間隔で発射
	if (m_bulletTime >= 3.0f)
	{
		// 弾を発射する
		Bullet* bulletTask = this->GetTaskManager()->AddTask<Bullet>(m_gameResources, GetPosition(), m_bodyRotate * m_turretRotate);
		// 親を変更する
		bulletTask->ChangeParent(this->GetTaskManager()->GetRootTask());
		SetVelocity(SimpleMath::Vector3::Zero);
		m_bulletTime = 0.0f;

		m_shotFlag = true;
	}

	// 煙の速度ベクトル
	SimpleMath::Vector3 velocity = WHITE_SMOKE_SPEED;
	m_smokeTime += elapsedTime;
	// 煙のエフェクトを発生させる
	if (m_smokeTime > 1.0f)
	{
		SimpleMath::Vector3 position = SimpleMath::Vector3(GetPosition().x, GetPosition().y - 0.2f, GetPosition().z);
		GetTaskManager()->AddTask<SmokeEffect>(position, velocity);
		m_smokeTime = 0.0f;
	}

}

// 衝突中関数
void EnemyTank::Hit()
{
	// 摩擦により停止したら
	if (GetVelocity() == SimpleMath::Vector3::Zero)
	{
		// 方向へのベクトルを求める
		SimpleMath::Vector3 v = GetPosition() - m_target->GetPosition();
		v.Normalize();
		// 向きベクトルを作成
		SimpleMath::Vector3 dir;
		dir = SimpleMath::Vector3::Transform(-v, m_bodyRotate);
		// ノックバックする
		AddForce(dir, m_target->GetHitForce());

		// 通常
		m_enemyState = EnemyState::Normal;
	}
}

void EnemyTank::OnHit_Bullet()
{
	// 停止させる
	SetVelocity(SimpleMath::Vector3::Zero);

	// 無敵時間を過ぎたら衝突状態へ
	if (m_invincibleTime > INVINCIBILITY_TIME)
	{
		m_enemyState = EnemyState::Hit;
		// 時間をリセット
		m_invincibleTime = 0.0f;
	}
}

void EnemyTank::OnHit_Player()
{
	SetVelocity(SimpleMath::Vector3::Zero);
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
DirectX::SimpleMath::Vector3 EnemyTank::Seek(const DirectX::SimpleMath::Vector3& targetPosition)
{
	// 目標位置への方向ベクトルの算出
	SimpleMath::Vector3 toTarget = targetPosition - GetPosition();

	// 期待速度の算出
	toTarget.Normalize();
	SimpleMath::Vector3 desiredVelocity = toTarget * ENEMY_MAX_SPEED;

	// 操舵力の算出
	SimpleMath::Vector3 steeringForce = desiredVelocity - m_velocity;

	return steeringForce;
}

// 逃走行動
DirectX::SimpleMath::Vector3 EnemyTank::Flee(const DirectX::SimpleMath::Vector3& targetPosition)
{
	// 目標位置への方向ベクトルの算出
	SimpleMath::Vector3 toTarget = targetPosition - GetPosition();

	// 期待速度の算出
	toTarget.Normalize();
	SimpleMath::Vector3 desiredVelocity = -toTarget * ENEMY_MAX_SPEED;

	// 操舵力の算出
	SimpleMath::Vector3 steeringForce = desiredVelocity - m_velocity;

	// 操舵力の返却
	return steeringForce;
}

// 徘徊行動
DirectX::SimpleMath::Vector3 EnemyTank::Wander(float* wanderOriantation, float wanderRadius, float wanderDistance, float wanderRotate)
{
	static std::random_device                    seedGenerator;
	static std::mt19937                          randomGenerator{ seedGenerator() };
	static std::uniform_real_distribution<float> distribution{ -1.0, 1.0 };

	// ターゲットへの角度を変化
	*wanderOriantation += (XM_PI * wanderRotate) * distribution(randomGenerator) * wanderDistance;

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


