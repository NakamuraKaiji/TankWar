#include "pch.h"
#include "Resources.h"

std::unique_ptr<Resources> Resources::m_resources = nullptr;

// Resourcesクラスのインスタンスを取得する
Resources* const Resources::GetInstance()
{
	if (m_resources == nullptr)
	{
		// Resourcesクラスのインスタンスを生成する
		m_resources.reset(new Resources());
	}
	// Resourcesクラスのインスタンスを返す
	return m_resources.get();
}

// リソースをロードする
void Resources::LoadResource()
{
	m_tankBody        = DirectX::Model::CreateFromCMO(m_device, L"Resources/cmo/TankBody.cmo", *m_graphics->GetFX());
	m_tankTurret      = DirectX::Model::CreateFromCMO(m_device, L"Resources/cmo/TankTurret.cmo", *m_graphics->GetFX());
	m_enemyTankBody   = DirectX::Model::CreateFromCMO(m_device, L"Resources/cmo/EnemyTankBody.cmo", *m_graphics->GetFX());
	m_enemyTankTurret = DirectX::Model::CreateFromCMO(m_device, L"Resources/cmo/EnemyTankTurret.cmo", *m_graphics->GetFX());
	m_skydome         = DirectX::Model::CreateFromCMO(m_device, L"Resources/cmo/Skydome.cmo", *m_graphics->GetFX());
	m_bullet		  = DirectX::Model::CreateFromCMO(m_device, L"Resources/cmo/Bullet.cmo", *m_graphics->GetFX());
}
