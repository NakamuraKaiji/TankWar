//*********************************************************************
//			概要	：プレイシーンクラス
// 
//			制作日	：10月3日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#ifndef PLAYER_SCENE_DEFINED
#define PLAYER_SCENE_DEFINED
#include "ImaseLib/SceneManager.h"
#include "UserResources.h"
#include "ImaseLib/TaskManager.h"
#include "ImaseLib/GridFloor.h"
#include "GameObjects/PlayerCamera.h"
#include "GameObjects/Stage.h"

class PlayScene :public Imase::Scene<UserResources>
{
public:

	// コンストラクタ
	PlayScene();

	// デストラクタ
	~PlayScene();

	// 初期化
	void Initialize() override;

	// 更新
	void Update(const DX::StepTimer& timer) override;

	// 描画
	void Render() override;

	// 終了処理
	void Finalize() override;

	// デバイスに依存するリソースを作成する関数
	void CreateDeviceDependentResources() override;

	// ウインドウサイズに依存するリソースを作成する関数
	void CreateWindowSizeDependentResources() override;

	// デバイスロストした時に呼び出される関数
	void OnDeviceLost() override;

private:

	// グリッドの床へのポインタ
	std::unique_ptr<Imase::GridFloor> m_gridFloor;

	// グラフィックスへのポインタ
	Graphics* m_graphics = Graphics::GetInstance();

	// プレイヤーカメラ
	PlayerCamera m_playerCamera;

	// スカイドームモデルへのポインタ
	std::shared_ptr<DirectX::Model> m_skydomeModel;

	// タスクマネージャー
	Imase::TaskManager m_taskManager;

	// ステージ
	Stage* m_stage;
private:

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// 射影行列
	DirectX::SimpleMath::Matrix m_proj;

};

#endif // !PLAYER_SCENE_DEFINED