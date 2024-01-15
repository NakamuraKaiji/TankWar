//*********************************************************************
//			概要	：砲弾クラス
// 
//			制作日	：10月18日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "CollisionManager.h"
#include "GameParameter.h"
#include "GameResources.h"

class Bullet : public GameObject
{
public:
	// コンストラクタ
	Bullet(const GameResources& gameResources,
		DirectX::SimpleMath::Vector3 position, 
		DirectX::SimpleMath::Quaternion rotate
	);

	// 初期化
	void Initialize();

	// 更新 
	bool Update(float elapsedTime) override;

	// 描画
	void Render() override;

	// リセット
	void Reset();

	// 衝突したら呼ばれる関数
	void OnHit(GameObject* object) override;

private:
	// グラフィックス
	Graphics* m_graphics = Graphics::GetInstance(); 

	// DeviceContextクラスのインスタンス
	ID3D11DeviceContext* m_context;

	// 砲弾モデル
	std::shared_ptr<DirectX::Model> m_bulletModel;

	// Collisionで作成したリソース
	GameResources m_gameResources;

	float m_num;

};

