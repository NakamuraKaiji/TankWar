#include "pch.h"
#include "EnemyTank.h"
#include "Utilities/Resources.h"
#include <random>

using namespace DirectX;


// �R���X�g���N�^
EnemyTank::EnemyTank(int x, int y)
	: GameObject(static_cast<int>(ObjectID::Enemy), x, y, ENEMY_RADIUS, ENEMY_FRICTION, PLAYER_WEIGHT)
	, m_bodyModel{}
	, m_turretModel{}
	, m_tankPos{}
	, m_bodyRotate{}
	, m_turretRotate{}
	, m_coolTime(0.0f)
	, m_velocity{}
	, m_mass(1.0f)
	, m_maxSpeed(1.0f)
	, m_maxForce(1.0f)
	, m_wanderOrientation(0.0f)
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

	m_tankPos = SimpleMath::Vector3(0.0f, 0.0f, 5.0f);

}

// �X�V
bool EnemyTank::Update(float elapsedTime)
{
	// ���Ǘ͂̒�`
	SimpleMath::Vector3 steeringforce = SimpleMath::Vector3::Zero;

	// �T���s��
	//steeringforce += Seek(m_playerPos);

	// �p�j�s��
	steeringforce += Wander(&m_wanderOrientation, 1.0f, 2.0f, 0.2f, elapsedTime);

	float radius = 7.0f;
	if (m_tankPos.Length() > radius)
	{
		steeringforce += Seek(SimpleMath::Vector3::Zero);
	}

	// ���̂ɗ^����ꂽ��[m/s2]
	SimpleMath::Vector3 force = steeringforce / elapsedTime;

	// ���i�^���̎��s
	// �͂̒���
	force = Truncate(force, m_maxForce);

	// �����x�̎Z�o(�����x = �� / ����)
	SimpleMath::Vector3 acceleration = force / m_mass;

	// ���x�̍X�V����ђ���
	m_velocity += acceleration * elapsedTime;
	m_velocity = Truncate(m_velocity, m_maxSpeed);

	// ���W�̍X�V
	m_tankPos += m_velocity * elapsedTime;

	// �i�s����������
	TurnHeading(m_velocity);

	// �G����v���C���[�Ɍ������x�N�g�����v�Z����
	SimpleMath::Vector3 toTarget = /*GetTarget()->GetPosition()*/ - m_tankPos;

	// ��]�p���v�Z����
	m_turretRotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::UnitY, atan2(toTarget.x, toTarget.z));
	
	// �C�e�̔��ˊԊu
	m_coolTime += elapsedTime;
	// �X�y�[�X�L�[�Ŕ���
	if (m_coolTime >= 2.0f)
	{
		//PlayScene::Shot(m_tankPos + SimpleMath::Vector3::Transform(
		//	SimpleMath::Vector3(0.0f, 0.0f, 1.5f), SimpleMath::Matrix::CreateFromQuaternion(m_bodyRotate * m_turretRotate)),
		//	m_bodyRotate * m_turretRotate
		//);
		m_coolTime = 0.0f;
	}

	// ���N���X�̍X�V�֐����Ăяo���Ĉړ�����
	GameObject::Update(elapsedTime);

	// �����𐧌�����
	LimitSpeed(ENEMY_MAX_SPEED);


	// �Փ˔���}�l�[�W���[�ɓo�^
	pCollisionManager.AddObject(this);

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
	SimpleMath::Matrix mat = SimpleMath::Matrix::CreateFromQuaternion(m_bodyRotate) * SimpleMath::Matrix::CreateTranslation(m_tankPos);
	m_parts[BODY]->SetTransformMatrix(mat);

	// ��Ԃ̕`��
	//SimpleMath::Matrix world;
	//world *= SimpleMath::Matrix::CreateScale(0.01f, 0.01f, 0.01f);
	m_parts[ROOT]->UpdateMatrix();
	m_parts[ROOT]->Draw(context, *states, view, proj);

}

void EnemyTank::OnHit(GameObject* object)
{
}

void EnemyTank::Hit()
{
}

void EnemyTank::OnHit_Bullet(GameObject* object)
{
}


// �I��
void EnemyTank::Finalize()
{
}

// �w��̕����Ɍ���
void EnemyTank::TurnHeading(const DirectX::SimpleMath::Vector3& direction)
{
	if (direction.Length() > 0.0001f)
	{
		SimpleMath::Quaternion rotation = SimpleMath::Quaternion::FromToRotation(SimpleMath::Vector3::Forward, direction);
		m_bodyRotate = rotation;
	}
}

// �T���s��
DirectX::SimpleMath::Vector3 EnemyTank::Seek(const DirectX::SimpleMath::Vector3& targetPos)
{
	// �ڕW�ʒu�ւ̕����x�N�g���̎Z�o
	SimpleMath::Vector3 toTarget = targetPos - m_tankPos;

	// ���ґ��x�̎Z�o
	toTarget.Normalize();
	SimpleMath::Vector3 desiredVelocity = toTarget * m_maxSpeed;

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
	SimpleMath::Vector3 wanderCenter = m_tankPos + SimpleMath::Vector3::Transform(SimpleMath::Vector3::Forward, m_bodyRotate);

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


