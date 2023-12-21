//*********************************************************************
//			�T�v	�F�^���N�N���X
// 
//			�����	�F10��4��
// 
//			�����	�FKaiji Nakamura
//*********************************************************************
#pragma once
#include "ImaseLib/TaskManager.h"
#include "GameObject.h"
#include "GameParameter.h"
#include "ImaseLib/ModelPart.h"
#include "CollisionManager.h"
#include "GameResources.h"

class PlayerTank : public GameObject
{
public:

	// �v���C���[�̏��
	enum class PlayerState
	{
		Normal,		// �ʏ�
		Hit,        // �Փ�
	};

private:

	// �Փ˒��֐�
	void Hit();

	// �C�e�Ƃ̏Փˊ֐�
	void OnHit_Bullet(GameObject* object);

	// �G�Ƃ̓����蔻��
	void OnHit_Player(GameObject* object);

public:

	// �ԑ̉�]��ݒ肷��֐�
	void SetBodyRotate(DirectX::SimpleMath::Quaternion rotate) { m_bodyRotate = rotate; }

	// �ԑ̉�]���擾����֐�
	DirectX::SimpleMath::Quaternion GetBodyRotate() { return m_bodyRotate; }

	// �C�g��]��ݒ肷��֐�
	void SetTurretRotate(DirectX::SimpleMath::Quaternion rotate) { m_turretRotate = rotate; }

	// �C�g��]���擾����֐�
	DirectX::SimpleMath::Quaternion GetTurretRotate() { return m_turretRotate; }

	// �v���C���[�̏�Ԃ��擾����֐�
	PlayerState GetState() { return m_playerState; }

public:

	// �R���X�g���N�^
	PlayerTank(const GameResources& gameResources, DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotate);

	// ������
	void Initialize();

	// �X�V
	bool Update(float elapsedTime) override;

	// �`��
	void Render() override;

	// �Փ˂�����Ă΂��֐�
	void OnHit(GameObject* object) override;

	// �ړ��֐�
	void Move(DirectX::Keyboard::KeyboardStateTracker* tracker);

	//	���Z�b�g
	void Reset();
private:

	// �O���t�B�b�N�X
	Graphics* m_graphics = Graphics::GetInstance();

	// ��Ԃ̊e�p�[�c�ւ̃|�C���^
	std::shared_ptr<DirectX::Model> m_bodyModel;
	std::shared_ptr<DirectX::Model> m_turretModel;
	enum {ROOT , BODY, TURRET, PARTS_CNT};

	// ��Ԋe�p�[�c�ւ̃|�C���^
	std::unique_ptr<Imase::ModelPart> m_parts[PARTS_CNT];

	// �v���C���[�̏��
	PlayerState m_playerState;

	// Collision�ō쐬�������\�[�X
	GameResources m_gameResources;

private:	
	// �ԑ̂̉�]
	DirectX::SimpleMath::Quaternion m_bodyRotate;

	// �C�g�̉�]
	DirectX::SimpleMath::Quaternion m_turretRotate;
};
