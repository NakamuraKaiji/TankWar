//*********************************************************************
//			概要	：プレイシーンクラス
// 
//			制作日	：10月3日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#include "pch.h"
#include "PlayScene.h"
#include "Utilities/Resources.h"
#include "GameObjects/GameResources.h"

using namespace DirectX;

// コンストラクタ
PlayScene::PlayScene()
	: m_view{}
	, m_proj{}
	, m_skydomeModel{}
	, m_stage{}
	, m_UI{}
	, m_life{}
{
}

// 初期化
void PlayScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// 残機数を設定
	m_life = PLAYER_CNT;
	m_UI->GetLife()->SetLife(m_life);
}

// 更新
void PlayScene::Update(const DX::StepTimer& timer)
{
	auto kbTracker = GetUserResources()->GetKeyboardStateTracker();

	// タスクの更新
	m_taskManager.Update(timer.GetElapsedSeconds());

	// 全ての敵を動かす
	m_stage->StopAllEnemy(false);

	// プレイヤーの移動処理
	m_stage->GetPlayer()->Move(kbTracker);

	// カメラの設定
	m_playerCamera.SetType(PlayerCamera::Type::TYPE_A);
	m_playerCamera.SetPlayer(m_stage->GetPlayer()->GetPosition(), m_stage->GetPlayer()->GetBodyRotate() * m_stage->GetPlayer()->GetTurretRotate());

	// カメラの更新
	m_playerCamera.Update(timer.GetElapsedSeconds());
	
	// 衝突判定の更新
	m_collisionManager.Update();

	if (kbTracker->pressed.Z)
	{
		m_life--;
		m_UI->GetLife()->SetLife(m_life);
	}
}

// 描画
void PlayScene::Render()
{
	auto debugFont = GetUserResources()->GetDebugFont();
	//debugFont->AddString(L"PlayScene", SimpleMath::Vector2(0.0f, debugFont->GetFontHeight()));

	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	m_view = SimpleMath::Matrix::CreateLookAt(
		m_playerCamera.GetEyePosition(),
		m_playerCamera.GetTargetPosition(),
		SimpleMath::Vector3::UnitY
	);

	// ビュー行列を設定
	m_graphics->SetViewMatrix(m_view);

	// スカイドームの描画
	SimpleMath::Matrix skyWorld;
	skyWorld = SimpleMath::Matrix::CreateTranslation(m_stage->GetPlayer()->GetPosition());
	m_skydomeModel->Draw(context, *states, skyWorld, m_view, m_proj);

	// グリッドの床を描画
	m_gridFloor->Render(context, m_view = m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());

	if (m_graphics->GetSpriteBatch()) m_graphics->GetSpriteBatch()->Begin();

	// タスクの描画
	m_taskManager.Render();

	if (m_graphics->GetSpriteBatch()) m_graphics->GetSpriteBatch()->End();

}

// 終了
void PlayScene::Finalize()
{
	// グリッドを終了
	m_gridFloor.reset();
}

// デバイスに依存するリソースを作成する関数
void PlayScene::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();
	// グラフィックスの生成
	m_graphics->GetInstance()->SetDeviceResources(GetUserResources()->GetDeviceResources());
	m_graphics->GetInstance()->SetCommonStates(states);
	m_graphics->Initialize();

	// リソースをロード
	Resources::GetInstance()->LoadResource();

	// グリッドの床を作成
	m_gridFloor = std::make_unique<Imase::GridFloor>(device, context, states);

	// スカイドームを作成
	m_skydomeModel = Resources::GetInstance()->GetSkydome();

	// ステージを作成 
	GameResources gameResources = {&m_collisionManager};
	if (m_stage) m_stage->Kill();
	m_stage = m_taskManager.AddTask<Stage>(gameResources);
	
	// ステージデータの設定
	m_stage->SetStageData();

	// UIの作成
	if (m_UI) m_UI->Kill();
	m_UI = m_taskManager.AddTask<UI>();
}

// ウインドウサイズに依存するリソースを作成する関数
void PlayScene::CreateWindowSizeDependentResources()
{
	// 射影行列を作成
	RECT rect = GetUserResources()->GetDeviceResources()->GetOutputSize();
	m_proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 200.0f
	);
	// 射影行列を設定
	m_graphics->SetProjectionMatrix(m_proj);
}

// デバイスロストした時に呼び出される関数
void PlayScene::OnDeviceLost()
{
	Finalize();
}

