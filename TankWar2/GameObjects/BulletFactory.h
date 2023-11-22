//*********************************************************************
//			概要	：砲弾ファクトリークラス
// 
//			制作日	：10月18日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#ifndef BULLET_FACTORY_DEFIEND
#define BULLET_FACTORY_DEFIEND
#include <vector>
#include "Interface/IBulletFactory.h"
#include "Bullet.h"

class BulletFactory : public IBulletFactory
{
public:
	// 指定された砲弾配列の生成
	static std::vector<std::unique_ptr<Bullet>> CreateBullets(
		const int& number,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Quaternion& rotate,
		const float& scale,
		bool used
	);
};


#endif // !BULLET_FACTORY_DEFIEND