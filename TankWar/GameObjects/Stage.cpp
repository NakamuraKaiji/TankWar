#include "pch.h"
#include "Stage.h"

using namespace DirectX;

// コンストラクタ
Stage::Stage(const GameResources& gameResources)
	: m_playerTask{}
	, m_enemyTask{}
	, m_gameResources(gameResources)
{
}

// 初期化
void Stage::Initialize()
{
}

// ステージ上のオブジェクトを全て削除
void Stage::DeleteStageObjects()
{
	// プレイヤーを削除する
	if (m_playerTask)
	{
		m_playerTask->Kill();
		m_playerTask = nullptr;
	}

}

// ステージを設定
void Stage::SetStageData()
{
	// プレイヤーを生成
	m_playerTask = GetTaskManager()->AddTask<PlayerTank>(m_gameResources, SimpleMath::Vector3::Zero, SimpleMath::Quaternion::Identity);

	// 敵を生成
	m_enemyTask = GetTaskManager()->AddTask<EnemyTank>(m_gameResources, SimpleMath::Vector3(0.0f, 0.0f, 5.0f), SimpleMath::Quaternion::Identity);

	// プレイヤーに敵を渡す
	m_playerTask->SetTarget(m_enemyTask);

	// 敵にプレイヤーを渡す
	m_enemyTask->SetTarget(m_playerTask);
}

// ステージデータのリセット
void Stage::Reset()
{
}

bool Stage::IsDeadAllEnemy()
{
	return false;
}
