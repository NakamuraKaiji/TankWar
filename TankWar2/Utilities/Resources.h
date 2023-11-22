//*********************************************************************
//			概要	：リソースクラス
// 
//			制作日	：6月14日
// 
//			制作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#ifndef RESOURCES_DEFINED
#define RESOURCES_DEFINED

class Resources
{
public:
	// 車体モデルを取得する
	std::shared_ptr<DirectX::Model> GetTankBody() { return m_tankBody; }
	// 砲身モデルを取得する
	std::shared_ptr<DirectX::Model> GetTankTurret() { return m_tankTurret; }
	// 敵の車体モデルを取得する
	std::shared_ptr<DirectX::Model> GetEnemyTankBody() { return m_enemyTankBody; }
	// 敵の砲身モデルを取得する
	std::shared_ptr<DirectX::Model> GetEnemyTankTurret() { return m_enemyTankTurret; }
	// スカイドームモデルを取得する
	std::shared_ptr<DirectX::Model> GetSkydome() { return m_skydome;}
	// 砲弾を取得する
	std::shared_ptr<DirectX::Model> GetBullet() { return m_bullet; }

public:
	// Resoucesクラスのインスタンスを取得する
	static Resources* const GetInstance();
	// リソースをロードする
	void LoadResource();
private:
	Resources() 
		: m_tankBody{}
		, m_tankTurret{}
		, m_enemyTankBody{}
		, m_enemyTankTurret{}
		, m_skydome{}
		, m_bullet{}
	{}
	// 代入は許容しない
	void operator=(const Resources& object) = delete;
	// コピーコンストラクタは許容しない
	Resources(const Resources& object) = delete;

private:
	// Resourcesクラスのインスタンスへのポインタ
	static std::unique_ptr<Resources> m_resources;
	// グラフィックス
	Graphics* m_graphics = Graphics::GetInstance();
	// デバイス
	ID3D11Device* m_device = m_graphics->GetDeviceResources()->GetD3DDevice();

	// 車体モデル
	std::shared_ptr <DirectX::Model> m_tankBody;
	// 砲身モデル
	std::shared_ptr<DirectX::Model> m_tankTurret;
	// 敵の車体モデル
	std::shared_ptr<DirectX::Model> m_enemyTankBody;
	// 敵の砲身モデル
	std::shared_ptr<DirectX::Model> m_enemyTankTurret;
	// スカイドームモデル
	std::shared_ptr<DirectX::Model> m_skydome;
	// 砲弾モデル
	std::shared_ptr<DirectX::Model> m_bullet;

};

#endif // RESOURCES_DEFINED
