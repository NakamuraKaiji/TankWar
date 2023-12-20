#include "pch.h"
#include "Bullet.h"
#include "Utilities/Resources.h"
#include "GameParameter.h"

using namespace DirectX;

// コンストラクタ
Bullet::Bullet(const GameResources& gameResources,
	DirectX::SimpleMath::Vector3 position,
	DirectX::SimpleMath::Quaternion rotate
)
	: GameObject(static_cast<int>(ObjectID::Bullet), position, rotate, BULLET_RADIUS, BULLET_FRICTION, BULLET_WEIGHT)
	, m_gameResources(gameResources)
	, m_context{}
{
}

// 初期化
void Bullet::Initialize()
{
	// デバイスコンテキストを取得
	m_context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	// 砲弾モデルを取得
	m_bulletModel = Resources::GetInstance()->GetBullet();

	// 描画順の設定
	SetOt(static_cast<int>(OT_Priority::OT_Object));
}

// 更新
bool Bullet::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	// 基底クラスの更新関数を呼び出して移動する
	GameObject::Update(elapsedTime);

	// 回転
	SimpleMath::Matrix rotation = SimpleMath::Matrix::CreateFromQuaternion(GetRotate());
	// 速度を計算
	SimpleMath::Vector3 velocity = SimpleMath::Vector3::TransformNormal(BULLET_SPEED, rotation);
	// 移動
	SetPosition(GetPosition() + velocity);

	if (GetPosition().x <= GetPosition().x - BULLET_COLLECT ||
		GetPosition().x >= GetPosition().x + BULLET_COLLECT ||
		GetPosition().z <= GetPosition().z - BULLET_COLLECT ||
		GetPosition().z >= GetPosition().z + BULLET_COLLECT)
	{
		this->Kill();
	}

	// 衝突判定用オブジェクト
	m_gameResources.pCollisionManager->AddObject(this);

	return true;
}

// 描画
void Bullet::Render()
{
	// ワールド行列を初期化
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;

	world *= SimpleMath::Matrix::CreateFromQuaternion(GetRotate());
 	world *= SimpleMath::Matrix::CreateTranslation(GetPosition().x, GetPosition().y, GetPosition().z);

	// プリミティブ描画を開始
	m_graphics->DrawPrimitiveBegin(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());
	// 「タンク」を描画
	m_graphics->DrawModel(m_bulletModel.get(), world);
	// プリミティブ描画を終了
	m_graphics->DrawPrimitiveEnd();
}

// リセット
void Bullet::Reset()
{
}

// 衝突したら呼ばれる関数
void Bullet::OnHit(GameObject* object)
{
	// 当たった相手が敵だったら消す
	if (object->GetID() == static_cast<int>(ObjectID::Enemy))
	{
		this->Kill();
	}
}

