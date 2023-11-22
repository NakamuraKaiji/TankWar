//*********************************************************************
//			�T�v	�F�C�e�}�l�[�W���[�N���X
// 
//			�����	�F11��2��
// 
//			�����	�FKaiji Nakamura
//*********************************************************************
#pragma once
#include "Bullet.h"

class BulletManager
{
private:
	// �C�e�z��
	std::vector<std::unique_ptr<Bullet>> m_bullets;

public:
	// �R���X�g���N�^
	BulletManager();

	// ������
	void Initialize(int number);

	// �X�V
	void Update(DX::StepTimer timer);

	// �`��
	void Render();

	// �I��
	void Finalize();

private:
	// �e������W
	float m_bulletRecovery;

};
