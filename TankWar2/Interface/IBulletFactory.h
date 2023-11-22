//*********************************************************************
//			概要	：砲弾ファクトリークラスのインターフェース
// 
//			制作日	：10月18日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#ifndef IBULLET_FACTORY_DEFINED
#define IBULLET_FACTORY_DEFINED
#include <vector>
#include "GameObjects/Bullet.h"

class IBulletFactory
{
	// 指定された砲弾配列を生成
	virtual std::vector<std::unique_ptr<Bullet>> CreateBullets(
		const int& number,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Quaternion& rotate,
		const float& scale,
		bool used
	) = 0;
};

#endif // !IBULLET_FACTORY_DEFINED