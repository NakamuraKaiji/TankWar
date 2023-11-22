#include "pch.h"
#include "Bullet.h"
#include "Utilities/Resources.h"
#include "GameParameter.h"

using namespace DirectX;

// コンストラクタ
Bullet::Bullet(int x, int y, 
	DirectX::SimpleMath::Vector3 position,
	DirectX::SimpleMath::Quaternion rotate,
	float scale
)
	: GameObject(static_cast<int>(ObjectID::Bullet), x, y, BULLET_RADIUS, BULLET_FRICTION, BULLET_WEIGHT)
	, m_context{}
	, m_position(position)
	, m_rotate(rotate)
	, m_scale(scale)
	, m_used(false)
{
}

// 初期化
void Bullet::Initialize()
{
	// デバイスコンテキストを取得
	m_context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	// 砲弾モデルを取得
	m_bulletModel = Resources::GetInstance()->GetBullet();
	
}

// 更新
bool Bullet::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	// 回転
	SimpleMath::Matrix rotation = SimpleMath::Matrix::CreateFromQuaternion(GetTurretRotate());
	// 速度を計算
	SimpleMath::Vector3 velocity = SimpleMath::Vector3::TransformNormal(BULLET_SPEED, rotation);
	// 移動
	m_position += velocity;

	SetBulletPosition(m_position);

	// 衝突判定用オブジェクト
	pCollisionManager.AddObject(this);

	return true;
}

// 描画
void Bullet::Render()
{
	// ワールド行列を初期化
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;

	world *= SimpleMath::Matrix::CreateScale(m_scale);
	world *= SimpleMath::Matrix::CreateFromQuaternion(GetTurretRotate());
 	world *= SimpleMath::Matrix::CreateTranslation(GetPosition().x, GetPosition().y, GetPosition().z);

	// プリミティブ描画を開始
	m_graphics->DrawPrimitiveBegin(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());
	// 「タンク」を描画
	m_graphics->DrawModel(m_bulletModel.get(), world);
	// プリミティブ描画を終了
	m_graphics->DrawPrimitiveEnd();
}

// 終了
void Bullet::Finalize()
{
}

// 衝突したら呼ばれる関数
void Bullet::OnHit(GameObject* object)
{
}

