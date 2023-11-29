#include "pch.h"
#include "PlayerTank.h"
#include "Utilities/Resources.h"
#include "Bullet.h"

using namespace DirectX;


// コンストラクタ
PlayerTank::PlayerTank(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotate)
	: GameObject(static_cast<int>(ObjectID::Player), position, rotate, PLAYER_RADIUS, PLAYER_FRICTION, PLAYER_WEIGHT)
	, m_bodyModel{}
	, m_turretModel{}
	, m_bodyRotate{}
	, m_turretRotate{}
{
}

// 初期化
void PlayerTank::Initialize()
{
	// 車体モデルを取得
	m_bodyModel = Resources::GetInstance()->GetTankBody();
	// 砲身モデルを取得
	m_turretModel = Resources::GetInstance()->GetTankTurret();

	// 戦車の各パーツの作成
	m_parts[ROOT]   = std::make_unique<Imase::ModelPart>();
	m_parts[BODY]   = std::make_unique<Imase::ModelPart>(m_bodyModel.get());
	m_parts[TURRET] = std::make_unique<Imase::ModelPart>(m_turretModel.get());
	// 各パーツを連結する
	// ROOTから車体の親子関係
	m_parts[ROOT]->SetChild(m_parts[BODY].get());
	// 車体から砲身の親子関係
	m_parts[BODY]->SetChild(m_parts[TURRET].get());
}

// 更新
bool PlayerTank::Update(float elapsedTime)
{
	// 基底クラスの更新関数を呼び出して移動する
	GameObject::Update(elapsedTime);

	// 速さを制限する
	 LimitSpeed(PLAYER_MAX_SPEED);

	// 衝突判定マネージャーに登録
	pCollisionManager.AddObject(this);

	return true;
}

// 描画
void PlayerTank::Render()
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
	SimpleMath::Matrix mat = SimpleMath::Matrix::CreateFromQuaternion(m_bodyRotate) * 
		SimpleMath::Matrix::CreateTranslation(GetPosition());
	m_parts[BODY]->SetTransformMatrix(mat);

	// 戦車の描画
	m_parts[ROOT]->UpdateMatrix();
	m_parts[ROOT]->Draw(context, *states, view, proj);

}

void PlayerTank::OnHit(GameObject* object)
{
}

void PlayerTank::Move(DirectX::Keyboard::KeyboardStateTracker* tracker)
{
	// キー入力を取得
	auto kb = Keyboard::Get().GetState();

	// 向いている方向に力を加えて移動する
	float force = PLAYER_MOVE_FORCE;

	// Wキーで前進
	if (kb.W)
	{
		AddForce(SimpleMath::Vector3::Transform(OBJECT_FORWARD, m_bodyRotate), -force);
	}
	// Sキーで後進
	if (kb.S)
	{
		AddForce(SimpleMath::Vector3::Transform(OBJECT_FORWARD, m_bodyRotate), force);
	}
	// Dキーで右回転
	if (kb.D)
	{
		// 回転角を計算
		m_bodyRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitY, DirectX::XMConvertToRadians(-PLAYER_BODY_ROTATE)
		);
	}
	// Aキーで左回転
	if (kb.A)
	{
		// 回転角を計算
		m_bodyRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitY, DirectX::XMConvertToRadians(PLAYER_BODY_ROTATE)
		);
	}
	// →で砲身を右回転
	if (kb.Right)
	{
		// 回転角を計算
		m_turretRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitY, DirectX::XMConvertToRadians(-PLAYER_TURRET_ROTATE)
		);
	}
	// ←で砲身を右回転
	if (kb.Left)
	{
		// 回転角を計算
		m_turretRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitY, DirectX::XMConvertToRadians(PLAYER_TURRET_ROTATE)
		);
	}

	// Spaceで発射
	if (tracker->pressed.Space)
	{
		// 弾を発射する
		Bullet* bulletTask = this->GetTaskManager()->AddTask<Bullet>(GetPosition(), m_bodyRotate * m_turretRotate, 0.5f);
		// 親を変更する
		bulletTask->ChangeParent(this->GetTaskManager()->GetRootTask());
	}

}

void PlayerTank::Hit()
{
}

void PlayerTank::OnHit_Bullet(GameObject* object)
{
}

// リセット
void PlayerTank::Reset()
{
}
