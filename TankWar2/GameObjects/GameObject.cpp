#include "pch.h"
#include "GameObject.h"
#include "GameParameter.h"

using namespace DirectX;

GameObject::GameObject(
	int id, 
	DirectX::SimpleMath::Vector3 position,
	DirectX::SimpleMath::Quaternion rotate,
	float radius, 
	float friction, 
	float weight
)
	: m_id(id)
	, m_radius(radius)
	, m_friction(friction)
	, m_weight(weight)
	, m_position(position)
	, m_rotate(rotate)
{
}

// 更新
bool GameObject::Update(float elapsedTime)
{
	// 摩擦力 = 摩擦係数 × 質量 × 重力加速度
	float frictionForce = m_friction * m_weight * 9.8f * elapsedTime;

	// 摩擦力によって速度を落とす
	float v = m_velocity.Length() - (frictionForce / m_weight);
	if (v < 0.0f) v = 0.0f;
	m_velocity.Normalize();
	m_velocity *= v;

	// 位置に速度を足す
	m_position += m_velocity;

	return true;
}

// 力を加える関数
void GameObject::AddForce(DirectX::SimpleMath::Vector3 dir, float force)
{
	// 加速度
	m_acceleration = dir * (force / m_weight);

	// 速度に加速度を足す
	m_velocity += m_acceleration;
}

// 速さを制限する関数
void GameObject::LimitSpeed(float speed)
{
	// 最大速度以上にならないよう調整
	if (m_velocity.LengthSquared() > speed * speed)
	{
		m_velocity.Normalize();
		m_velocity *= speed;
	}
}

// 衝突時の力を取得する
float GameObject::GetHitForce()
{
	float force = HIT_FORCE_MIN;

	// 連続ヒット時の速度の影響が大きすぎるので速度を抑える
	float v = m_velocity.Length();
	if (v > 0.1f) v = 0.1f;
	force += m_weight * v;

	return force;
}
