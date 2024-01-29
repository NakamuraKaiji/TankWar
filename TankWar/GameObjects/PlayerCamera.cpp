#include "pch.h"
#include "PlayerCamera.h"

using namespace DirectX;

PlayerCamera::PlayerCamera()
	: m_type(Type::TYPE_A)
	, m_playerPos{}
	, m_playerRotate{}
	, m_angle(0.0f)
{
}

void PlayerCamera::Update(float elapsedTime)
{
	switch (m_type)
	{
	case PlayerCamera::Type::TYPE_A:					// プレイヤーを後ろから追いかけるカメラ
	{
		SimpleMath::Vector3 targetPos = m_playerPos + SimpleMath::Vector3(0.0f, 0.5f, 0.0f);
		SimpleMath::Vector3 eyePos(0.0f, 0.8f, -3.0f);
		eyePos = SimpleMath::Vector3::Transform(eyePos, m_playerRotate);
		SetPositionTarget(targetPos + eyePos, targetPos);
	}
		break;
	case PlayerCamera::Type::TYPE_B:					// プレイヤーを中心に回るカメラ
	{
		m_angle += XMConvertToRadians(15.0f) * elapsedTime;
		SimpleMath::Vector3 targetPos = m_playerPos + SimpleMath::Vector3(0.0f, 0.1f, 0.0f);
		SimpleMath::Vector3 eyePos(0.0f, 1.0f, 3.0f);
		eyePos = SimpleMath::Vector3::Transform(eyePos, SimpleMath::Matrix::CreateRotationY(m_angle));
		SetPositionTarget(targetPos + eyePos, targetPos);
	}
		break;

	case PlayerCamera::Type::TYPE_C:
	{
		SimpleMath::Vector3 targetPos = SimpleMath::Vector3(0.0f, 0.5f, 0.0f);;
		SimpleMath::Vector3 eyePos = SimpleMath::Vector3(0.0f, 0.8f, 2.0f);
		SetPositionTarget(eyePos, targetPos);
	}

	default:
		break;
	}
}
