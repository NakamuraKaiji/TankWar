#include "pch.h"
#include "BulletManager.h"

using namespace DirectX;

// コンストラクタ
BulletManager::BulletManager()
	: m_bulletRecovery(100.0f)
{
}

// デストラクタ
void BulletManager::Initialize(int number)
{
	// 配列のサイズを設定
	m_bullets.resize(number);

	// 配列に砲弾を格納
	for (int i = 0; i < number; i++)   
	{
		// 砲弾を生成
		m_bullets[i] = std::make_unique<Bullet>(0, 0, SimpleMath::Vector3::Zero, SimpleMath::Quaternion::Identity, 0.5f);

		// 砲弾を初期化
		m_bullets[i]->Initialize();
	}
}

// 更新
void BulletManager::Update(DX::StepTimer timer)
{
	// 飛弾中の砲弾の更新と境界線の砲弾を初期化
	for (int i = 0; i < m_bullets.size(); i++)
	{
		//m_bullets[i]->used();

		// 発射されているかどうか調べる
		if (!m_bullets[i]->GetUsed()) { continue; }

		// 砲弾を更新
		m_bullets[i]->Update(timer);

		// 砲弾を回収
		if (m_bullets[i]->GetBulletPos().x <= -m_bulletRecovery ||
			m_bullets[i]->GetBulletPos().x >=  m_bulletRecovery ||
			m_bullets[i]->GetBulletPos().z <= -m_bulletRecovery ||
			m_bullets[i]->GetBulletPos().z >=  m_bulletRecovery)
		{
			// 砲弾を未使用にする
			m_bullets[i]->SetUsed(false);
		}
	}
}

// 描画
void BulletManager::Render()
{
	for (int i = 0; i < m_bullets.size(); i++)
	{
		// 発射されているかどうか調べる
		if (!m_bullets[i]->GetUsed()) { continue; }

		// 砲弾を描画
		m_bullets[i]->Render();
	}

}

// 終了
void BulletManager::Finalize()
{
	//	弾の最終処理
	for (int i = 0; i < m_bullets.size(); i++)
	{
		m_bullets[i]->Finalize();
	}
}
