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

#include <unordered_map>

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
	// 砲弾モデルを取得する
	std::shared_ptr<DirectX::Model> GetBullet() { return m_bullet; }
	// 負けた戦車モデルを取得する
	std::shared_ptr<DirectX::Model> GetLoseTank() { return m_loseTank; }
	// 地面モデルを取得する
	std::shared_ptr<DirectX::Model> GetGround() { return m_ground; }
	// 山モデルを取得する
	std::shared_ptr<DirectX::Model> GetMountain() { return m_mountain; }
	// 木モデルを取得する
	std::shared_ptr<DirectX::Model> GetTree() { return m_tree; }

	// パーティクルのテクスチャを取得する
	ID3D11ShaderResourceView* GetParticle() { return m_particleTexture.Get(); }
	// 残機のテクスチャを取得する
	ID3D11ShaderResourceView* GetLife() { return m_life.Get(); }
	// PushEnterテクスチャを取得する
	ID3D11ShaderResourceView* GetPush() { return m_push.Get(); }
	// タイトルテクスチャを取得する
	ID3D11ShaderResourceView* GetTitle() { return m_title.Get(); }
	// 勝利テクスチャを取得する
	ID3D11ShaderResourceView* GetVictory() { return m_victory.Get(); }
	// 敗北テクスチャを取得する
	ID3D11ShaderResourceView* GetDefeat() { return m_defeat.Get(); }
	// HPテクスチャAを取得する
	ID3D11ShaderResourceView* GetHP_A() { return m_hp_a.Get(); }
	// HPテクスチャBを取得する
	ID3D11ShaderResourceView* GetHP_B() { return m_hp_b.Get(); }
	// 数字テクスチャ1を取得する
	ID3D11ShaderResourceView* GetNumber1() { return m_number1.Get(); }
	// 数字テクスチャ2を取得する
	ID3D11ShaderResourceView* GetNumber2() { return m_number2.Get(); }
	// 数字テクスチャ3を取得する
	ID3D11ShaderResourceView* GetNumber3() { return m_number3.Get(); }

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
		, m_ground{}
		, m_particleTexture{}
		, m_loseTank{}
		, m_push{}
		, m_title{}
		, m_victory{}
		, m_defeat{}
		, m_mountain{}
		, m_tree{}
		, m_hp_a{}
		, m_hp_b{}
		, m_number1{}
		, m_number2{}
		, m_number3{}
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
	// 負けた体勢の戦車モデル
	std::shared_ptr<DirectX::Model> m_loseTank;
	// 地面モデル
	std::shared_ptr<DirectX::Model> m_ground;
	// 山モデル
	std::shared_ptr<DirectX::Model> m_mountain;
	// 木モデル
	std::shared_ptr<DirectX::Model> m_tree;

	// パーティクルのテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_particleTexture;
	// 残機のテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_life;
	// PushEnterテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_push;
	// タイトルテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_title;
	// 勝利テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_victory;
	// 敗北テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_defeat;
	// 体力テクスチャA
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_hp_a;
	// 体力テクスチャB
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_hp_b;
	//	数字テクスチャ1
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_number1;
	//	数字テクスチャ2
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_number2;
	//	数字テクスチャ3
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_number3;

};

#endif // RESOURCES_DEFINED
