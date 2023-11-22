//*********************************************************************
//			�T�v	�F�C�e�N���X
// 
//			�����	�F10��18��
// 
//			�����	�FKaiji Nakamura
//*********************************************************************
#pragma once
#ifndef BULLET_DEFIEND
#define BULLET_DEFIEND
#include "CollisionManager.h"
#include "GameParameter.h"

class Bullet : GameObject
{
public:
	// ���˃t���O�̎擾
	bool GetUsed() { return m_used; }

	// ���˃t���O�̐ݒ�
	void SetUsed(bool used) { m_used = used; }

	// �ʒu���擾
	DirectX::SimpleMath::Vector3 GetBulletPos() { return m_position; }

	// �ʒu��ݒ�
	void SetBulletPos(DirectX::SimpleMath::Vector3 position) { m_position = position; }

public:
	// �R���X�g���N�^
	Bullet(int x, int y,
		DirectX::SimpleMath::Vector3 position, 
		DirectX::SimpleMath::Quaternion rotate, 
		float scale
	);

	// ������
	void Initialize();

	// �X�V 
	bool Update(float elapsedTime) override;

	// �`��
	void Render() override;

	// �I��
	void Finalize();

	// �Փ˂�����Ă΂��֐�
	void OnHit(GameObject* object) override;

	void used();

private:
	// �O���t�B�b�N�X
	Graphics* m_graphics = Graphics::GetInstance(); 

	// �Փ˔���p�I�u�W�F�N�g
	CollisionManager pCollisionManager;

	// DeviceContext�N���X�̃C���X�^���X
	ID3D11DeviceContext* m_context;

	// �C�e���f��
	std::shared_ptr<DirectX::Model> m_bulletModel;

private:
	// �ʒu
	DirectX::SimpleMath::Vector3 m_position;

	// �p�x
	DirectX::SimpleMath::Quaternion m_rotate;

	// �傫��
	float m_scale;

	// �C�e�����˂���Ă��邩�ǂ���������
	bool m_used;

};

#endif // !BULLET_DEFIEND


