#include "pch.h"
#include "UserInterface.h"
#include "Utilities/Resources.h"

// コンストラクタ
UserInterface::UserInterface()
	: m_lifeTexture{}
	, m_life(nullptr)
	, m_enemyHP(nullptr)
{
	m_lifeTexture = Resources::GetInstance()->GetLife();
}

// 初期化
void UserInterface::Initialize()
{
	// 残機数タスク登録
	m_life = GetTaskManager()->AddTask<Life>(
		m_lifeTexture.Get(), 10, 10
		);

	// 敵HPタスク登録
	m_enemyHP = GetTaskManager()->AddTask<EnemyHP>();
}