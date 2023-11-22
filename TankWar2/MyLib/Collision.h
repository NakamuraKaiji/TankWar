//*********************************************************************
//			概要	：当たり判定クラス
// 
//			制作日	：10月25日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#ifndef COLLISION_DEFINED
#define COLLISION_DEFINED

class Collision
{
public:
	// コンストラクタ
	Collision();
	// デストラクタ
	~Collision();
	// 初期化
	void Initialize();
	// 更新
	void Update(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotate);
	// 衝突判定
	bool Intersects();

private:
	// 衝突判定情報
	std::vector<DirectX::BoundingBox*> m_srcBoxes;

	// 衝突判定情報（ワールド空間）
	std::vector<DirectX::BoundingOrientedBox> m_dstOrientedBoxes;

};

#endif // !COLLISION_DEFINED
