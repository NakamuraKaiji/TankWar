#include "pch.h"
#include "Resources.h"

std::unique_ptr<Resources> Resources::m_resources = nullptr;

// Resources�N���X�̃C���X�^���X���擾����
Resources* const Resources::GetInstance()
{
	if (m_resources == nullptr)
	{
		// Resources�N���X�̃C���X�^���X�𐶐�����
		m_resources.reset(new Resources());
	}
	// Resources�N���X�̃C���X�^���X��Ԃ�
	return m_resources.get();
}

// ���\�[�X�����[�h����
void Resources::LoadResource()
{
	m_tankBody        = DirectX::Model::CreateFromCMO(m_device, L"Resources/cmo/TankBody.cmo", *m_graphics->GetFX());
	m_tankTurret      = DirectX::Model::CreateFromCMO(m_device, L"Resources/cmo/TankTurret.cmo", *m_graphics->GetFX());
	m_enemyTankBody   = DirectX::Model::CreateFromCMO(m_device, L"Resources/cmo/EnemyTankBody.cmo", *m_graphics->GetFX());
	m_enemyTankTurret = DirectX::Model::CreateFromCMO(m_device, L"Resources/cmo/EnemyTankTurret.cmo", *m_graphics->GetFX());
	m_skydome         = DirectX::Model::CreateFromCMO(m_device, L"Resources/cmo/Skydome.cmo", *m_graphics->GetFX());
	m_bullet		  = DirectX::Model::CreateFromCMO(m_device, L"Resources/cmo/Bullet.cmo", *m_graphics->GetFX());
}
