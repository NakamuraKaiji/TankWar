#include "pch.h"
#include "EnemyTank.h"
#include "Utilities/Resources.h"
#include "Bullet.h"
#include <random>

using namespace DirectX;

// �R���X�g���N�^
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

// ������
void EnemyTank::Initialize()
{
	// �ԑ̃��f�����擾
	m_bodyModel = Resources::GetInstance()->GetEnemyTankBody();
	// �C�g���f�����擾
	m_turretModel = Resources::GetInstance()->GetEnemyTankTurret();

	// �e�p�[�c�̍쐬
	m_parts[ROOT]   = std::make_unique<Imase::ModelPart>();
	m_parts[BODY]   = std::make_unique<Imase::ModelPart>(m_bodyModel.get());
	m_parts[TURRET] = std::make_unique<Imase::ModelPart>(m_turretModel.get());
	// �e�p�[�c��A������
	// ROOT����ԑ̂̐e�q�֌W
	m_parts[ROOT]->SetChild(m_parts[BODY].get());
	// �ԑ̂���C�g�̐e�q�֌W
	m_parts[BODY]->SetChild(m_parts[TURRET].get());

	m_wanderOrientation = 0.0f;

	// �`�揇�̐ݒ�
	SetOt(static_cast<int>(OT_Priority::OT_Object));
}

// �X�V
bool EnemyTank::Update(float elapsedTime)
{
	// ��Ԃɂ���ď����𕪊򂳂���
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

	// ���N���X�̍X�V�֐����Ăяo���Ĉړ�����
	GameObject::Update(elapsedTime);

	// �����𐧌�����
	LimitSpeed(ENEMY_MAX_SPEED);

	// �Փ˔���}�l�[�W���[�ɓo�^
	m_gameResources.pCollisionManager->AddObject(this);

	return true;
}

// �`��
void EnemyTank::Render()
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_graphics->GetCommonStates();

	// �r���[�s��Ǝˉe�s����擾
	SimpleMath::Matrix view, proj;
	view = m_graphics->GetViewMatrix();
	proj = m_graphics->GetProjectionMatrix();

	// ��Ԃ̉�]
	m_parts[BODY]->SetTransformMatrix(SimpleMath::Matrix::CreateFromQuaternion(m_bodyRotate));
	// �C�g�̉�]
	m_parts[TURRET]->SetTransformMatrix(SimpleMath::Matrix::CreateFromQuaternion(m_turretRotate));
	// ��Ԃ̈ړ�
	SimpleMath::Matrix mat = SimpleMath::Matrix::CreateFromQuaternion(m_bodyRotate) 
						   * SimpleMath::Matrix::CreateTranslation(GetPosition());
	m_parts[BODY]->SetTransformMatrix(mat);

	// ��Ԃ̕`��
	m_parts[ROOT]->UpdateMatrix();
	m_parts[ROOT]->Draw(context, *states, view, proj);

}

// �Փ˂�����Ă΂��֐�
void EnemyTank::OnHit(GameObject* object)
{
	// ��~������
	SetVelocity(SimpleMath::Vector3::Zero);
	// �ړ�������
	SimpleMath::Vector3 dir = GetPosition() - object->GetPosition();
	dir.y = 0.0f;
	dir.Normalize();
	AddForce(dir, object->GetHitForce());
	// �Փˏ�Ԃ�
	m_enemyState = EnemyState::Hit;
}

// ���Z�b�g
void EnemyTank::Reset()
{
}

// �ʏ펞�̊֐�
void EnemyTank::Normal(float elapsedTime)
{
	// ���Ǘ͂̒�`
	SimpleMath::Vector3 steeringforce = SimpleMath::Vector3::Zero;
	// �T���s��
	//steeringforce += Seek(m_target->GetPosition());

	// �p�j�s��
	steeringforce += Wander(&m_wanderOrientation, 1.0f, 2.0f, 0.2f, elapsedTime);

	SimpleMath::Vector3 toTarget = GetPosition() - m_target->GetPosition();

	float radius = 5.0f;
	if (toTarget.Length() > radius)
	{
		steeringforce -= Seek(m_target->GetPosition());
	}

	// ���̂ɗ^����ꂽ��[m/s2]
	SimpleMath::Vector3 force = steeringforce / elapsedTime;

	// ���i�^���̎��s
	// �͂̒���
	force = Truncate(force, ENEMY_MOVE_FORCE);

	// �����x�̎Z�o(�����x = �� / ����)
	SimpleMath::Vector3 acceleration = force / m_mass;

	// ���x�̍X�V����ђ���
	m_velocity += acceleration * elapsedTime;
	m_velocity = Truncate(m_velocity, ENEMY_MAX_SPEED);

	AddForce(SimpleMath::Vector3::Transform(-OBJECT_FORWARD, m_bodyRotate), ENEMY_MOVE_FORCE);

	// ���W�̍X�V
	SetPosition(GetPosition() - (m_velocity * elapsedTime));

	// �i�s����������
	TurnHeading(m_velocity);

	// ��]�p���v�Z����
	//m_turretRotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, atan2(toTarget.z, toTarget.x));

	// �C�e�̔��ˊԊu
	m_interval += elapsedTime;
	// 2�b�Ԋu�Ŕ���
	if (m_interval >= 2.0f)
	{
		// �e�𔭎˂���
		Bullet* bulletTask = this->GetTaskManager()->AddTask<Bullet>(m_gameResources, GetPosition(), m_bodyRotate * m_turretRotate);
		// �e��ύX����
		bulletTask->ChangeParent(this->GetTaskManager()->GetRootTask());

		SetVelocity(SimpleMath::Vector3::Zero);

		m_interval = 0.0f;
	}
}

// �Փ˒��֐�
void EnemyTank::Hit()
{
	// ���C�ɂ���~������
	if (GetVelocity() == SimpleMath::Vector3::Zero)
	{
		m_enemyState = EnemyState::Normal;
	}
}

// �w��̕����Ɍ���
void EnemyTank::TurnHeading(const DirectX::SimpleMath::Vector3& direction)
{
	if (direction.Length() > 0.001f)
	{
		SimpleMath::Quaternion rotation = SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::Forward, direction);
		m_bodyRotate = rotation;
	}
}

// �T���s��
DirectX::SimpleMath::Vector3 EnemyTank::Seek(const DirectX::SimpleMath::Vector3& targetPos)
{
	// �ڕW�ʒu�ւ̕����x�N�g���̎Z�o
	SimpleMath::Vector3 toTarget = targetPos - GetPosition();

	// ���ґ��x�̎Z�o
	toTarget.Normalize();
	SimpleMath::Vector3 desiredVelocity = toTarget * ENEMY_MAX_SPEED;

	// ���Ǘ͂̎Z�o
	SimpleMath::Vector3 steeringForce = desiredVelocity - m_velocity;

	return steeringForce;
}

// �p�j�s��
DirectX::SimpleMath::Vector3 EnemyTank::Wander(float* wanderOriantation, float wanderRadius, float wanderDistance, float wanderRotate, float time)
{
	static std::random_device                    seedGenerator;
	static std::mt19937                          randomGenerator{ seedGenerator() };
	static std::uniform_real_distribution<float> distribution{ -1.0, 1.0 };

	// �^�[�Q�b�g�ւ̊p�x��ω�
	*wanderOriantation += (XM_PI * wanderRotate) * distribution(randomGenerator);

	// �p�j�s���p�~�̉~����̃^�[�Q�b�g�̈ʒu�̉��o
	SimpleMath::Vector3 targetOriantation = SimpleMath::Vector3{ std::cos(*wanderOriantation), 0.0f, std::sin(*wanderOriantation) };
	SimpleMath::Vector3 toTarget = targetOriantation * wanderRadius;

	// �p�j�s���p�̒��S�̎Z�o
	SimpleMath::Vector3 wanderCenter = GetPosition() + SimpleMath::Vector3::Transform(SimpleMath::Vector3::Forward, m_bodyRotate);

	// �^�[�Q�b�g�̈ʒu�̎Z�o(���[���h���W)
	SimpleMath::Vector3 targetPosition = wanderCenter + toTarget;

	// �T���s���ɂ�鑀�Ǘ͂̎Z�o
	SimpleMath::Vector3 steeringForce = Seek(targetPosition);

	return steeringForce;
}

// �x�N�g���̒����̐؂�̂�
inline DirectX::SimpleMath::Vector3 EnemyTank::Truncate(const DirectX::SimpleMath::Vector3& v, float maxLength)
{
	const float maxLengthSquared = maxLength * maxLength;
	const float vecLengthSquared = v.LengthSquared();
	if (vecLengthSquared <= maxLengthSquared)
		return v;
	else
		return v * (maxLength / v.Length());
}


