#include "pch.h"
#include "GameObject.h"
#include "GameParameter.h"

using namespace DirectX;

GameObject::GameObject(
	int id, 
	int x, int y, 
	float radius, 
	float friction, 
	float weight
)
	: m_id(id)
	, m_x(x), m_y(y)
	, m_radius(radius)
	, m_friction(friction)
	, m_weight(weight)
{
	// 位置を設定
	m_position = SimpleMath::Vector3(static_cast<float>(m_x), 0.0f, static_cast<float>(m_y));
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

void GameObject::Shot(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion bodyRotate, DirectX::SimpleMath::Quaternion turretRotate)
{
	// 位置を設定
 	m_bulletPosition = position + SimpleMath::Vector3::Transform(
		SimpleMath::Vector3(0.0f, 0.0f, 1.5f), SimpleMath::Matrix::CreateFromQuaternion(bodyRotate * turretRotate));

	// 回転角を設定
	m_bulletRotate = bodyRotate * turretRotate;
}
