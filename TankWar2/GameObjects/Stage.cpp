#include "pch.h"
#include "Stage.h"

using namespace DirectX;

// �R���X�g���N�^
Stage::Stage()
	: m_playerTask{}
	, m_enemyTask{}
{
}

// ������
void Stage::Initialize()
{
}

// �X�e�[�W��̃I�u�W�F�N�g��S�č폜����֐�
void Stage::DeleteStageObjects()
{
	// �v���C���[���폜����
	if (m_playerTask)
	{
		m_playerTask->Kill();
		m_playerTask = nullptr;
	}

}

void Stage::SetStageData()
{
	// �v���C���[�𐶐�
	m_playerTask = GetTaskManager()->AddTask<PlayerTank>(0, 0);

	// �G�𐶐�
	m_enemyTask = GetTaskManager()->AddTask<EnemyTank>(0, 0);
}

void Stage::Finalize()
{
}

void Stage::StopAllEnemy(bool isStop)
{
}

bool Stage::IsDeadAllEnemy()
{
	return false;
}
