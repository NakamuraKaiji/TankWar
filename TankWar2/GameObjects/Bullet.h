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

class Bullet : GameObject
{
public:
	// 発射フラグの取得
	bool GetUsed() { return m_used; }

	// 発射フラグの設定
	void SetUsed(bool used) { m_used = used; }

	// 位置を取得
	DirectX::SimpleMath::Vector3 GetBulletPos() { return m_position; }

	// 位置を設定
	void SetBulletPos(DirectX::SimpleMath::Vector3 position) { m_position = position; }

public:
	// コンストラクタ
	Bullet(int x, int y,
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

	// 終了
	void Finalize();

	// 衝突したら呼ばれる関数
	void OnHit(GameObject* object) override;

	void used();

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

	// 角度
	DirectX::SimpleMath::Quaternion m_rotate;

	// 大きさ
	float m_scale;

	// 砲弾が発射されているかどうかを示す
	bool m_used;

};

#endif // !BULLET_DEFIEND


