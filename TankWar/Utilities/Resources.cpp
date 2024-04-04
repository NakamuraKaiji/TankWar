#include "pch.h"
#include "Resources.h"

using namespace DirectX;

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
	// モデル
	m_tankBody        = Model::CreateFromCMO(m_device, L"Resources/cmo/TankBody.cmo", *m_graphics->GetFX());
	m_tankTurret      = Model::CreateFromCMO(m_device, L"Resources/cmo/TankTurret.cmo", *m_graphics->GetFX());
	m_enemyTankBody   = Model::CreateFromCMO(m_device, L"Resources/cmo/EnemyTankBody.cmo", *m_graphics->GetFX());
	m_enemyTankTurret = Model::CreateFromCMO(m_device, L"Resources/cmo/EnemyTankTurret.cmo", *m_graphics->GetFX());
	m_skydome         = Model::CreateFromCMO(m_device, L"Resources/cmo/Skydome.cmo", *m_graphics->GetFX());
	m_bullet		  = Model::CreateFromCMO(m_device, L"Resources/cmo/Bullet.cmo", *m_graphics->GetFX());
	m_loseTank		  = Model::CreateFromCMO(m_device, L"Resources/cmo/LoseTank.cmo", *m_graphics->GetFX());
	m_ground          = Model::CreateFromCMO(m_device, L"Resources/cmo/Ground.cmo", *m_graphics->GetFX());
	m_mountain        = Model::CreateFromCMO(m_device, L"Resources/cmo/Mountain.cmo", *m_graphics->GetFX());

	// テクスチャ
	DX::ThrowIfFailed(CreateDDSTextureFromFile(m_graphics->GetDeviceResources()->GetD3DDevice(),
		L"Resources/dds/smoke_effect.dds",	nullptr, m_particleTexture.ReleaseAndGetAddressOf())
	);
	DX::ThrowIfFailed(CreateDDSTextureFromFile(m_graphics->GetDeviceResources()->GetD3DDevice(),
		L"Resources/dds/Life.dds", nullptr, m_life.ReleaseAndGetAddressOf())
	);
	DX::ThrowIfFailed(CreateDDSTextureFromFile(m_graphics->GetDeviceResources()->GetD3DDevice(),
		L"Resources/dds/PushEnter.dds", nullptr, m_push.ReleaseAndGetAddressOf())
	);
	DX::ThrowIfFailed(CreateDDSTextureFromFile(m_graphics->GetDeviceResources()->GetD3DDevice(),
		L"Resources/dds/TankWar.dds", nullptr, m_title.ReleaseAndGetAddressOf())
	);
	DX::ThrowIfFailed(CreateDDSTextureFromFile(m_graphics->GetDeviceResources()->GetD3DDevice(),
		L"Resources/dds/Victory.dds", nullptr, m_victory.ReleaseAndGetAddressOf())
	);
	DX::ThrowIfFailed(CreateDDSTextureFromFile(m_graphics->GetDeviceResources()->GetD3DDevice(),
		L"Resources/dds/Defeat.dds", nullptr, m_defeat.ReleaseAndGetAddressOf())
	);
	DX::ThrowIfFailed(CreateDDSTextureFromFile(m_graphics->GetDeviceResources()->GetD3DDevice(),
		L"Resources/dds/Defeat.dds", nullptr, m_defeat.ReleaseAndGetAddressOf())
	);
}