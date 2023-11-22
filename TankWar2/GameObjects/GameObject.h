//*********************************************************************
//			�T�v	�F�Q�[���I�u�W�F�N�g�N���X
// 
//			�����	�F11��2��
// 
//			�����	�FKaiji Nakamura
//*********************************************************************
#pragma once
#include "ImaseLib/TaskManager.h"
#include "GameParameter.h"

class GameObject : public Imase::Task
{
private:
	// �I�u�W�F�N�g����ID
	int m_id;

	// �����z�u�ʒu
	int m_x, m_y;

	// ���a
	float m_radius;

	// ����
	float m_weight;

	// ���Ƃ̖��C�W��
	float m_friction;

	// �����x
	DirectX::SimpleMath::Vector3 m_acceleration;

	// ���x
	DirectX::SimpleMath::Vector3 m_velocity;

	// �ʒu
	DirectX::SimpleMath::Vector3 m_position;

	// �ԑ̂̉�]
	DirectX::SimpleMath::Quaternion m_bodyRotate;

	// �C�g�̉�]
	DirectX::SimpleMath::Quaternion m_turretRotate;

	// �C�e�̈ʒu
	DirectX::SimpleMath::Vector3 m_bulletPosition;

	// �C�e�̉�]
	DirectX::SimpleMath::Quaternion m_bulletRotate;

public:

	// �R���X�g���N�^
	GameObject(int id, int x, int y, float radius, float friction, float weight);

	// �X�V
	bool Update(float elapsedTime) override;
	// �I�u�W�F�N�g����ID��ݒ�
	void SetID(int id) { m_id = id; }

	// �I�u�W�F�N�g����ID���擾
	int GetID() { return m_id; }

	// �Փ˂�����Ă΂��֐�
	virtual void OnHit(GameObject* object) = 0;

	// ���a���擾����֐�
	float GetRadius() { return m_radius; }

	// ���x��ݒ肷��֐�
	void SetVelocity(DirectX::SimpleMath::Vector3 velocity) { m_velocity = velocity; }

	// ���x���擾����֐�
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_velocity; }

	// ��Ԃ̈ʒu��ݒ肷��֐�
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }

	// ��Ԃ̈ʒu���擾����֐�
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }

	// �ԑ̉�]��ݒ肷��֐�
	void SetBodyRotate(DirectX::SimpleMath::Quaternion rotate) { m_bodyRotate = rotate; }

	// �ԑ̉�]���擾����֐�
	DirectX::SimpleMath::Quaternion GetBodyRotate() { return m_bodyRotate; }

	// �C�g��]��ݒ肷��֐�
	void SetTurretRotate(DirectX::SimpleMath::Quaternion rotate) { m_turretRotate = rotate; }

	// �C�g��]���擾����֐�
	DirectX::SimpleMath::Quaternion GetTurretRotate() { return m_turretRotate; }

	// �͂�������֐�
	void AddForce(DirectX::SimpleMath::Vector3 dir, float force);

	// ���C�W����ݒ肷��֐�
	void SetFriction(float friction) { m_friction = friction; }

	// �d����ݒ肷��֐�
	void SetWeight(float weight) { m_weight = weight; }

	// �����𐧌�����֐�
	void LimitSpeed(float speed);

	// �X�e�[�W��ł̔z�u�ʒu���擾����֐�
	int GetX() { return m_x; }
	int GetY() { return m_y; }

	// �ʒu��ݒ肷��֐�
	void SetBulletPosition(DirectX::SimpleMath::Vector3 position) { m_bulletPosition = position; }

	// �ʒu���擾����֐�
	const DirectX::SimpleMath::Vector3& GetBulletPosition() { return m_bulletPosition; }

	// �C�e��]���擾����֐�
	DirectX::SimpleMath::Quaternion GetBulletRotate() { return m_bulletRotate; }

	// �C�e��]��ݒ肷��֐�
	void SetBulletRotate(DirectX::SimpleMath::Quaternion rotate) { m_bulletRotate = rotate; }

	// �C�e���ˊ֐�
	void Shot(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion bodyRotate, DirectX::SimpleMath::Quaternion turretRotate);
};
