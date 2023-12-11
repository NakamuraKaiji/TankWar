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
#include "GameResources.h"

class EnemyTank : public GameObject
{
public:
	// �G�̏��
	enum class EnemyState
	{
		Normal,			// �ʏ�
		Hit,			// ������΂�����
	};

public:

	// �^�[�Q�b�g��ݒ�
	void SetTarget(GameObject* target) { m_target = target; }

	// ��Ԃ��擾����֐�
	EnemyState GetState() { return m_enemyState; }
private:

	// �ʏ펞�̊֐�
	void Normal(float elapsedTime);

	// �Փ˒��֐�
	void Hit();

	// �C�e�Ƃ̏Փˊ֐�
	void OnHit_Bullet(GameObject* object);

public:

	// �R���X�g���N�^
	EnemyTank(const GameResources& gameResources, DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotate);

	// ������
	void Initialize();

	// �X�V
	bool Update(float elapsedTime) override;

	// �`��
	void Render() override;

	// �Փ˂�����Ă΂��֐�
	void OnHit(GameObject* object) override;

	//	���Z�b�g
	void Reset();

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

	// �^�[�Q�b�g
	GameObject* m_target;

	// ��Ԃ̊e�p�[�c�ւ̃|�C���^
	std::shared_ptr<DirectX::Model> m_bodyModel;
	std::shared_ptr<DirectX::Model> m_turretModel;
	enum { ROOT, BODY, TURRET, PARTS_CNT };
	// ��Ԋe�p�[�c�ւ̃|�C���^
	std::unique_ptr<Imase::ModelPart> m_parts[PARTS_CNT];

	// �G�̏��
	EnemyState m_enemyState;

	// Collision�ō쐬�������\�[�X
	GameResources m_gameResources;

private:
	// �ԑ̂̊p�x
	DirectX::SimpleMath::Quaternion m_bodyRotate;
	// �C�g�̊p�x
	DirectX::SimpleMath::Quaternion m_turretRotate;

	// ���x[m/s]
	DirectX::SimpleMath::Vector3 m_velocity;
	// ����[kg]
	float m_mass;    
	// �p�j�s���Ŏg�p�������
	float m_wanderOrientation;   

	// �C�e�̔��ˊԊu
	float m_interval;
};

#endif // !ENEMY_TANK_DEFINED

