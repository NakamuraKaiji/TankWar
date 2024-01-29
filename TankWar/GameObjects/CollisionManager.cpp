#include "pch.h"
#include "CollisionManager.h"

using namespace DirectX;

// XV
void CollisionManager::Update()
{
	// ‘“–‚½‚è‚ÅÕ“Ë”»’è‚·‚é
	for (size_t i = 0; i < m_objects.size(); i++)
	{
		GameObject* a = m_objects[i];
		for (size_t j = i + 1; j < m_objects.size(); j++)
		{
			GameObject* b = m_objects[j];
			// ‹…‚Æ‹…‚ÌÕ“Ë”»’è
			SimpleMath::Vector3 distance = a->GetPosition() - b->GetPosition();
			float r = a->GetRadius() + b->GetRadius();
			if (distance.LengthSquared() <= r * r)
			{
				// Õ“Ë‚µ‚½
				a->OnHit(b);
				b->OnHit(a);
			}
		}
	}
	// “o˜^ƒŠƒXƒg‚ğƒNƒŠƒA‚É‚·‚é
	m_objects.clear();
}

// “o˜^ŠÖ”
void CollisionManager::AddObject(GameObject* object)
{
	// Õ“Ë”»’èƒŠƒXƒg‚É“o˜^
	m_objects.push_back(object);
}
