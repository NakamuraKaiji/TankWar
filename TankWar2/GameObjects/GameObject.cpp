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
	// �ʒu��ݒ�
	m_position = SimpleMath::Vector3(static_cast<float>(m_x), 0.0f, static_cast<float>(m_y));
}

// �X�V
bool GameObject::Update(float elapsedTime)
{
	// ���C�� = ���C�W�� �~ ���� �~ �d�͉����x
	float frictionForce = m_friction * m_weight * 9.8f * elapsedTime;

	// ���C�͂ɂ���đ��x�𗎂Ƃ�
	float v = m_velocity.Length() - (frictionForce / m_weight);
	if (v < 0.0f) v = 0.0f;
	m_velocity.Normalize();
	m_velocity *= v;

	// �ʒu�ɑ��x�𑫂�
	m_position += m_velocity;

	return true;
}

// �͂�������֐�
void GameObject::AddForce(DirectX::SimpleMath::Vector3 dir, float force)
{
	// �����x
	m_acceleration = dir * (force / m_weight);

	// ���x�ɉ����x�𑫂�
	m_velocity += m_acceleration;
}

// �����𐧌�����֐�
void GameObject::LimitSpeed(float speed)
{
	// �ő呬�x�ȏ�ɂȂ�Ȃ��悤����
	if (m_velocity.LengthSquared() > speed * speed)
	{
		m_velocity.Normalize();
		m_velocity *= speed;
	}
}

void GameObject::Shot(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion bodyRotate, DirectX::SimpleMath::Quaternion turretRotate)
{
	// �ʒu��ݒ�
 	m_bulletPosition = position + SimpleMath::Vector3::Transform(
		SimpleMath::Vector3(0.0f, 0.0f, 1.5f), SimpleMath::Matrix::CreateFromQuaternion(bodyRotate * turretRotate));

	// ��]�p��ݒ�
	m_bulletRotate = bodyRotate * turretRotate;
}
