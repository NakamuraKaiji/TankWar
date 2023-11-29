//*********************************************************************
//			概要	：砲弾クラス
// 
//			制作日	：10月18日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#ifndef BULLET_DEFIEND
#define BULLET_DEFIEND
#include "CollisionManager.h"
#include "GameParameter.h"

class Bullet : public GameObject
{
public:
	// コンストラクタ
	Bullet(
		DirectX::SimpleMath::Vector3 position, 
		DirectX::SimpleMath::Quaternion rotate, 
		float scale
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

	// 衝突判定用オブジェクト
	CollisionManager pCollisionManager;

	// DeviceContextクラスのインスタンス
	ID3D11DeviceContext* m_context;

	// 砲弾モデル
	std::shared_ptr<DirectX::Model> m_bulletModel;

private:
	// 位置
	DirectX::SimpleMath::Vector3 m_position;

	// 大きさ
	float m_scale;
};

#endif // !BULLET_DEFIEND


