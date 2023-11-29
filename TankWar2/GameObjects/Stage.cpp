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
	m_playerTask = GetTaskManager()->AddTask<PlayerTank>(SimpleMath::Vector3::Zero, SimpleMath::Quaternion::Identity);

	// 敵を生成
	m_enemyTask = GetTaskManager()->AddTask<EnemyTank>(SimpleMath::Vector3(0.0f, 0.0f, 5.0f), SimpleMath::Quaternion::Identity);

	// 敵にプレイヤーを渡す
	m_enemyTask->SetTarget(m_playerTask);
}

// ステージデータのリセット
void Stage::Reset()
{
}

void Stage::StopAllEnemy(bool isStop)
{
}

bool Stage::IsDeadAllEnemy()
{
	return false;
}
