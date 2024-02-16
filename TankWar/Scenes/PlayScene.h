//*********************************************************************
//			概要	：プレイシーンクラス
// 
//			制作日	：10月3日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "MyLib/SceneManager.h"
#include "UserResources.h"
#include "MyLib/TaskManager.h"
#include "GameObjects/PlayerCamera.h"
#include "GameObjects/Stage.h"
#include "GameObjects/CollisionManager.h"
#include "GameObjects/UI/UserInterface.h"

class PlayScene :public Scene<UserResources>
{
public:

	// コンストラクタ
	PlayScene();

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

	// グラフィックスへのポインタ
	Graphics* m_graphics = Graphics::GetInstance();

	// プレイヤーカメラ
	PlayerCamera m_playerCamera;

	// スカイドームモデルへのポインタ
	std::shared_ptr<DirectX::Model> m_skydomeModel;

	// 地面モデルへのポインタ
	std::shared_ptr<DirectX::Model> m_groundModel;

	// タスクマネージャー
	TaskManager m_taskManager;

	// ステージ
	Stage* m_stage;

	// 衝突判定用オブジェクト
	CollisionManager m_collisionManager;

	// UI
	UserInterface* m_userInterface;

	// BGM
	std::unique_ptr<DirectX::SoundEffectInstance> m_bgm;
	// 爆発音
	std::unique_ptr<DirectX::SoundEffectInstance> m_explosionSound;
	// 発射音
	std::unique_ptr<DirectX::SoundEffectInstance> m_shotSound;

private:

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// 射影行列
	DirectX::SimpleMath::Matrix m_proj;

	// 残機数
	int m_life;

	// 敵のHP
	float m_ratio;

	// スカイドームの回転
	float m_skydomeRotate;

};