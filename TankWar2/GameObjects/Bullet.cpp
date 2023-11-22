#include "pch.h"
#include "Bullet.h"
#include "Utilities/Resources.h"
#include "GameParameter.h"

using namespace DirectX;

// �R���X�g���N�^
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

// ������
void Bullet::Initialize()
{
	// �f�o�C�X�R���e�L�X�g���擾
	m_context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	// �C�e���f�����擾
	m_bulletModel = Resources::GetInstance()->GetBullet();
	
}

// �X�V
bool Bullet::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	// ��]
	SimpleMath::Matrix rotation = SimpleMath::Matrix::CreateFromQuaternion(GetTurretRotate());
	// ���x���v�Z
	SimpleMath::Vector3 velocity = SimpleMath::Vector3::TransformNormal(BULLET_SPEED, rotation);
	// �ړ�
	m_position += velocity;

	SetBulletPosition(m_position);

	// �Փ˔���p�I�u�W�F�N�g
	pCollisionManager.AddObject(this);

	return true;
}

// �`��
void Bullet::Render()
{
	// ���[���h�s���������
	SimpleMath::Matrix world = SimpleMath::Matrix::Identity;

	world *= SimpleMath::Matrix::CreateScale(m_scale);
	world *= SimpleMath::Matrix::CreateFromQuaternion(GetTurretRotate());
 	world *= SimpleMath::Matrix::CreateTranslation(GetPosition().x, GetPosition().y, GetPosition().z);

	// �v���~�e�B�u�`����J�n
	m_graphics->DrawPrimitiveBegin(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());
	// �u�^���N�v��`��
	m_graphics->DrawModel(m_bulletModel.get(), world);
	// �v���~�e�B�u�`����I��
	m_graphics->DrawPrimitiveEnd();
}

// �I��
void Bullet::Finalize()
{
}

// �Փ˂�����Ă΂��֐�
void Bullet::OnHit(GameObject* object)
{
}

