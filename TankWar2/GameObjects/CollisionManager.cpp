#include "pch.h"
#include "CollisionManager.h"

using namespace DirectX;

// 更新
void CollisionManager::Update()
{
	// 総当たりで衝突判定する
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		GameObject* a = m_objects[i];
		for (size_t j = i + 1; j < m_objects.size(); j++)
		{
			GameObject* b = m_objects[j];
			// 球と球の衝突判定
			SimpleMath::Vector3 distance = a->GetPosition() - b->GetPosition();
			float r = a->GetRadius() + b->GetRadius();
			if (distance.LengthSquared() <= r * r)
			{
				// 衝突した
				a->OnHit(b);
				b->OnHit(a);
			}
		}
	}
	// 登録リストをクリアにする
	m_objects.clear();
}

// 登録関数
void CollisionManager::AddObject(GameObject* object)
{
	// 衝突判定リストに登録
	m_objects.push_back(object);
}
