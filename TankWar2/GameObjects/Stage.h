//*********************************************************************
//			概要	：ステージクラス
// 
//			制作日	：11月14日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once

#include <vector>
#include "ImaseLib/TaskManager.h"
#include "PlayerTank.h"
#include "EnemyTank.h"

class Stage : public Imase::Task
{
private:

	// グラフィックス
	Graphics* m_graphics = Graphics::GetInstance();

	// プレイヤータスクのポインタ
	PlayerTank* m_playerTask;

	// 敵タスクのポインタ
	EnemyTank* m_enemyTask;

private:

	// ステージ上のオブジェクトを全て削除
	void DeleteStageObjects();


public:

	// コンストラクタ
	Stage();

	// 初期化
	void Initialize() override;

	// ステージを設定
	void SetStageData();

	// リセット
	void Reset();

	// プレイヤーを取得
	PlayerTank* GetPlayer() { return m_playerTask; }

	// 全ての敵を停止
	void StopAllEnemy(bool isStop);

	// 全ての敵が死んでいるかチェック
	bool IsDeadAllEnemy();

};