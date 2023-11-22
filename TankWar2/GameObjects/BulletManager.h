//*********************************************************************
//			概要	：砲弾マネージャークラス
// 
//			制作日	：11月2日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "Bullet.h"

class BulletManager
{
private:
	// 砲弾配列
	std::vector<std::unique_ptr<Bullet>> m_bullets;

public:
	// コンストラクタ
	BulletManager();

	// 初期化
	void Initialize(int number);

	// 更新
	void Update(DX::StepTimer timer);

	// 描画
	void Render();

	// 終了
	void Finalize();

private:
	// 弾回収座標
	float m_bulletRecovery;

};
