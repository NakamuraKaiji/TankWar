//*********************************************************************
//			概要	：衝突判定クラス
// 
//			制作日	：11月2日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "GameObject.h"

class CollisionManager
{
private:
	// 登録リスト
	std::vector<GameObject*> m_objects;

public:
	// コンストラクタ
	CollisionManager() = default;

	// 更新
	void Update();

	// 登録関数
	void AddObject(GameObject* object);
};