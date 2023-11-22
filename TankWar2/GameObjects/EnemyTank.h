//*********************************************************************
//			�T�v	�F�G�^���N�̃N���X
// 
//			�����	�F10��17��
// 
//			�����	�FKaiji Nakamura
//*********************************************************************
#pragma once
#ifndef ENEMY_TANK_DEFINED
#define ENEMY_TANK_DEFIEND
#include "GameObject.h"
#include "GameParameter.h"
#include "ImaseLib/ModelPart.h"
#include "CollisionManager.h"

class EnemyTank : public GameObject
{
private:

	// �Փ˒��֐�
	void Hit();

	// �C�e�Ƃ̏Փˊ֐�
	void OnHit_Bullet(GameObject* object);

public:

	// �R���X�g���N�^
	EnemyTank(int x, int y);

	// ������
	void Initialize();

	// �X�V
	bool Update(float elapsedTime) override;

	// �`��
	void Render() override;

	// �Փ˂�����Ă΂��֐�
	void OnHit(GameObject* object) override;

	//	�I��
	void Finalize();

	// �w��̕���������
	void TurnHeading(const DirectX::SimpleMath::Vector3& direction);

public:
	// ���Ǎs��
	// �T��
	DirectX::SimpleMath::Vector3 Seek(const DirectX::SimpleMath::Vector3& targetPos);

	// �p�j
	DirectX::SimpleMath::Vector3 Wander(float* wanderOriantation, float wanderRadius, float wanderDistance, float wanderRotate,  float time);

	// �x�N�g���̒����̐؂�̂�
	static DirectX::SimpleMath::Vector3 Truncate(const DirectX::SimpleMath::Vector3& v, float maxLength);
private:
	// �O���t�B�b�N�X
	Graphics* m_graphics = Graphics::GetInstance();

	// �Փ˔���p�I�u�W�F�N�g
	CollisionManager pCollisionManager;

	// ��Ԃ̊e�p�[�c�ւ̃|�C���^
	std::shared_ptr<DirectX::Model> m_bodyModel;
	std::shared_ptr<DirectX::Model> m_turretModel;
	enum { ROOT, BODY, TURRET, PARTS_CNT };
	// ��Ԋe�p�[�c�ւ̃|�C���^

	std::unique_ptr<Imase::ModelPart> m_parts[PARTS_CNT];

private:
	// �ԑ̂̈ʒu
	DirectX::SimpleMath::Vector3 m_tankPos;
	// �ԑ̂̊p�x
	DirectX::SimpleMath::Quaternion m_bodyRotate;
	// �C�g�̊p�x
	DirectX::SimpleMath::Quaternion m_turretRotate;

	// ���x[m/s]
	DirectX::SimpleMath::Vector3 m_velocity;
	// ����[kg]
	float m_mass;    
	// �ő�ړ����x(����)[m/s]
	float m_maxSpeed;   
	// �ő��[m/s2]
	float m_maxForce;   
	// �p�j�s���Ŏg�p�������
	float m_wanderOrientation;   


	// �C�e�̔��ˊԊu
	float m_coolTime;

};

#endif // !ENEMY_TANK_DEFINED

