#include "pch.h"
#include "Stage.h"

using namespace DirectX;

// コンストラクタ
Stage::Stage()
	: m_playerTask{}
	, m_enemyTask{}
{
}

// 初期化
void Stage::Initialize()
{
}

// ステージ上のオブジェクトを全て削除する関数
void Stage::DeleteStageObjects()
{
	// プレイヤーを削除する
	if (m_playerTask)
	{
		m_playerTask->Kill();
		m_playerTask = nullptr;
	}

}

void Stage::SetStageData()
{
	// プレイヤーを生成
	m_playerTask = GetTaskManager()->AddTask<PlayerTank>(0, 0);

	// 敵を生成
	m_enemyTask = GetTaskManager()->AddTask<EnemyTank>(0, 0);
}

void Stage::Finalize()
{
}

void Stage::StopAllEnemy(bool isStop)
{
}

bool Stage::IsDeadAllEnemy()
{
	return false;
}
