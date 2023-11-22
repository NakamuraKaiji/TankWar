//*********************************************************************
//			�T�v	�F�X�e�[�W�N���X
// 
//			�����	�F11��14��
// 
//			�����	�FKaiji Nakamura
//*********************************************************************
#pragma once

#include <vector>
#include "ImaseLib/TaskManager.h"
#include "PlayerTank.h"
#include "EnemyTank.h"

class Stage : public Imase::Task
{
private:

	// �O���t�B�b�N�X
	Graphics* m_graphics = Graphics::GetInstance();

	// �v���C���[�^�X�N�̃|�C���^
	PlayerTank* m_playerTask;

	// �G�^�X�N�̃|�C���^
	EnemyTank* m_enemyTask;

private:

	// �X�e�[�W��̃I�u�W�F�N�g��S�č폜����֐�
	void DeleteStageObjects();


public:

	// �R���X�g���N�^
	Stage();

	// ������
	void Initialize() override;

	// �X�e�[�W��ݒ肷��֐�
	void SetStageData();

	// �I��
	void Finalize();

	// �v���C���[���擾
	PlayerTank* GetPlayer() { return m_playerTask; }

	// �S�Ă̓G���~����֐�
	void StopAllEnemy(bool isStop);

	// �S�Ă̓G������ł��邩�`�F�b�N����֐�
	bool IsDeadAllEnemy();

};