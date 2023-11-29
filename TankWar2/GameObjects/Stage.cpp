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

// �X�e�[�W��̃I�u�W�F�N�g��S�č폜
void Stage::DeleteStageObjects()
{
	// �v���C���[���폜����
	if (m_playerTask)
	{
		m_playerTask->Kill();
		m_playerTask = nullptr;
	}

}

// �X�e�[�W��ݒ�
void Stage::SetStageData()
{
	// �v���C���[�𐶐�
	m_playerTask = GetTaskManager()->AddTask<PlayerTank>(SimpleMath::Vector3::Zero, SimpleMath::Quaternion::Identity);

	// �G�𐶐�
	m_enemyTask = GetTaskManager()->AddTask<EnemyTank>(SimpleMath::Vector3(0.0f, 0.0f, 5.0f), SimpleMath::Quaternion::Identity);

	// �G�Ƀv���C���[��n��
	m_enemyTask->SetTarget(m_playerTask);
}

// �X�e�[�W�f�[�^�̃��Z�b�g
void Stage::Reset()
{
}

void Stage::StopAllEnemy(bool isStop)
{
}

bool Stage::IsDeadAllEnemy()
{
	return false;
}
