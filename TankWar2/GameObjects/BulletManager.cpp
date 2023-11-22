#include "pch.h"
#include "BulletManager.h"

using namespace DirectX;

// �R���X�g���N�^
BulletManager::BulletManager()
	: m_bulletRecovery(100.0f)
{
}

// �f�X�g���N�^
void BulletManager::Initialize(int number)
{
	// �z��̃T�C�Y��ݒ�
	m_bullets.resize(number);

	// �z��ɖC�e���i�[
	for (int i = 0; i < number; i++)   
	{
		// �C�e�𐶐�
		m_bullets[i] = std::make_unique<Bullet>(0, 0, SimpleMath::Vector3::Zero, SimpleMath::Quaternion::Identity, 0.5f);

		// �C�e��������
		m_bullets[i]->Initialize();
	}
}

// �X�V
void BulletManager::Update(DX::StepTimer timer)
{
	// ��e���̖C�e�̍X�V�Ƌ��E���̖C�e��������
	for (int i = 0; i < m_bullets.size(); i++)
	{
		//m_bullets[i]->used();

		// ���˂���Ă��邩�ǂ������ׂ�
		if (!m_bullets[i]->GetUsed()) { continue; }

		// �C�e���X�V
		m_bullets[i]->Update(timer);

		// �C�e�����
		if (m_bullets[i]->GetBulletPos().x <= -m_bulletRecovery ||
			m_bullets[i]->GetBulletPos().x >=  m_bulletRecovery ||
			m_bullets[i]->GetBulletPos().z <= -m_bulletRecovery ||
			m_bullets[i]->GetBulletPos().z >=  m_bulletRecovery)
		{
			// �C�e�𖢎g�p�ɂ���
			m_bullets[i]->SetUsed(false);
		}
	}
}

// �`��
void BulletManager::Render()
{
	for (int i = 0; i < m_bullets.size(); i++)
	{
		// ���˂���Ă��邩�ǂ������ׂ�
		if (!m_bullets[i]->GetUsed()) { continue; }

		// �C�e��`��
		m_bullets[i]->Render();
	}

}

// �I��
void BulletManager::Finalize()
{
	//	�e�̍ŏI����
	for (int i = 0; i < m_bullets.size(); i++)
	{
		m_bullets[i]->Finalize();
	}
}
