#include "pch.h"
#include "BulletManager.h"

using namespace DirectX;

// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
BulletManager::BulletManager()
	: m_bulletRecovery(100.0f)
{
}

// ƒfƒXƒgƒ‰ƒNƒ^
void BulletManager::Initialize(int number)
{
	// ”z—ñ‚ÌƒTƒCƒY‚ğİ’è
	m_bullets.resize(number);

	// ”z—ñ‚É–C’e‚ğŠi”[
	for (int i = 0; i < number; i++)   
	{
		// –C’e‚ğ¶¬
		m_bullets[i] = std::make_unique<Bullet>(0, 0, SimpleMath::Vector3::Zero, SimpleMath::Quaternion::Identity, 0.5f);

		// –C’e‚ğ‰Šú‰»
		m_bullets[i]->Initialize();
	}
}

// XV
void BulletManager::Update(DX::StepTimer timer)
{
	// ”ò’e’†‚Ì–C’e‚ÌXV‚Æ‹«ŠEü‚Ì–C’e‚ğ‰Šú‰»
	for (int i = 0; i < m_bullets.size(); i++)
	{
		//m_bullets[i]->used();

		// ”­Ë‚³‚ê‚Ä‚¢‚é‚©‚Ç‚¤‚©’²‚×‚é
		if (!m_bullets[i]->GetUsed()) { continue; }

		// –C’e‚ğXV
		m_bullets[i]->Update(timer);

		// –C’e‚ğ‰ñû
		if (m_bullets[i]->GetBulletPos().x <= -m_bulletRecovery ||
			m_bullets[i]->GetBulletPos().x >=  m_bulletRecovery ||
			m_bullets[i]->GetBulletPos().z <= -m_bulletRecovery ||
			m_bullets[i]->GetBulletPos().z >=  m_bulletRecovery)
		{
			// –C’e‚ğ–¢g—p‚É‚·‚é
			m_bullets[i]->SetUsed(false);
		}
	}
}

// •`‰æ
void BulletManager::Render()
{
	for (int i = 0; i < m_bullets.size(); i++)
	{
		// ”­Ë‚³‚ê‚Ä‚¢‚é‚©‚Ç‚¤‚©’²‚×‚é
		if (!m_bullets[i]->GetUsed()) { continue; }

		// –C’e‚ğ•`‰æ
		m_bullets[i]->Render();
	}

}

// I—¹
void BulletManager::Finalize()
{
	//	’e‚ÌÅIˆ—
	for (int i = 0; i < m_bullets.size(); i++)
	{
		m_bullets[i]->Finalize();
	}
}
