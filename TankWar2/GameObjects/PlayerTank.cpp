#include "pch.h"
#include "PlayerTank.h"
#include "Utilities/Resources.h"

using namespace DirectX;


// �R���X�g���N�^
PlayerTank::PlayerTank(int x, int y)
	: GameObject(static_cast<int>(ObjectID::Player), x, y, PLAYER_RADIUS, PLAYER_FRICTION, PLAYER_WEIGHT)
	, m_bodyModel{}
	, m_turretModel{}
	, m_position{}
	, m_bodyRotate{}
	, m_turretRotate{}
{
}

// ������
void PlayerTank::Initialize()
{
	// �ԑ̃��f�����擾
	m_bodyModel = Resources::GetInstance()->GetTankBody();
	// �C�g���f�����擾
	m_turretModel = Resources::GetInstance()->GetTankTurret();

	// ��Ԃ̊e�p�[�c�̍쐬
	m_parts[ROOT]   = std::make_unique<Imase::ModelPart>();
	m_parts[BODY]   = std::make_unique<Imase::ModelPart>(m_bodyModel.get());
	m_parts[TURRET] = std::make_unique<Imase::ModelPart>(m_turretModel.get());
	// �e�p�[�c��A������
	// ROOT����ԑ̂̐e�q�֌W
	m_parts[ROOT]->SetChild(m_parts[BODY].get());
	// �ԑ̂���C�g�̐e�q�֌W
	m_parts[BODY]->SetChild(m_parts[TURRET].get());
}

// �X�V
bool PlayerTank::Update(float elapsedTime)
{
	// ���N���X�̍X�V�֐����Ăяo���Ĉړ�����
	GameObject::Update(elapsedTime);

	// �����𐧌�����
	 LimitSpeed(PLAYER_MAX_SPEED);

	// �Փ˔���}�l�[�W���[�ɓo�^
	pCollisionManager.AddObject(this);

	return true;
}

// �`��
void PlayerTank::Render()
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_graphics->GetCommonStates();

	// �r���[�s��Ǝˉe�s����擾
	SimpleMath::Matrix view, proj;
	view = m_graphics->GetViewMatrix();
	proj = m_graphics->GetProjectionMatrix();

	// ��Ԃ̉�]
	m_parts[BODY]->SetTransformMatrix(SimpleMath::Matrix::CreateFromQuaternion(GetBodyRotate()));
	// �C�g�̉�]
	m_parts[TURRET]->SetTransformMatrix(SimpleMath::Matrix::CreateFromQuaternion(GetTurretRotate()));
	// ��Ԃ̈ړ�
	SimpleMath::Matrix mat = SimpleMath::Matrix::CreateFromQuaternion(GetBodyRotate()) * 
		SimpleMath::Matrix::CreateTranslation(GetPosition());
	m_parts[BODY]->SetTransformMatrix(mat);

	// ��Ԃ̕`��
	m_parts[ROOT]->UpdateMatrix();
	m_parts[ROOT]->Draw(context, *states, view, proj);

}

void PlayerTank::OnHit(GameObject* object)
{
}

void PlayerTank::Move(DirectX::Keyboard::KeyboardStateTracker* tracker)
{
	// �L�[���͂��擾
	auto kb = Keyboard::Get().GetState();

	// �����Ă�������ɗ͂������Ĉړ�����
	float force = PLAYER_MOVE_FORCE;

	// W�L�[�őO�i
	if (kb.W)
	{
		AddForce(SimpleMath::Vector3::Transform(OBJECT_FORWARD, GetBodyRotate()), -force);
	}
	// S�L�[�Ō�i
	if (kb.S)
	{
		AddForce(SimpleMath::Vector3::Transform(OBJECT_FORWARD, GetBodyRotate()), force);
	}
	// D�L�[�ŉE��]
	if (kb.D)
	{
		// ��]�p���v�Z
		m_bodyRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitY, DirectX::XMConvertToRadians(-PLAYER_BODY_ROTATE)
		);
	}
	// A�L�[�ō���]
	if (kb.A)
	{
		// ��]�p���v�Z
		m_bodyRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitY, DirectX::XMConvertToRadians(PLAYER_BODY_ROTATE)
		);
	}
	// ���ŖC�g���E��]
	if (kb.Right)
	{
		// ��]�p���v�Z
		m_turretRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitY, DirectX::XMConvertToRadians(-PLAYER_TURRET_ROTATE)
		);
	}
	// ���ŖC�g���E��]
	if (kb.Left)
	{
		// ��]�p���v�Z
		m_turretRotate *= SimpleMath::Quaternion::CreateFromAxisAngle(
			SimpleMath::Vector3::UnitY, DirectX::XMConvertToRadians(PLAYER_TURRET_ROTATE)
		);
	}

	SetBodyRotate(m_bodyRotate);
	SetTurretRotate(m_turretRotate);

	// Space�Ŕ���
	if (tracker->pressed.Space)
	{
 		Shot(GetPosition(), GetBodyRotate(), GetTurretRotate());
	}

}

void PlayerTank::Hit()
{
}

void PlayerTank::OnHit_Bullet(GameObject* object)
{
}

// �I��
void PlayerTank::Finalize()
{
}
