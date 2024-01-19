#include "pch.h"
#include "ResultScene.h"
#include "Utilities/Resources.h"
#include "TitleScene.h"

using namespace DirectX;

// コンストラクタ
ResultScene::ResultScene()
	: m_view{}
	, m_proj{}
	, m_count(0)
	, m_rotate{}
{
}

// 初期化
void ResultScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// カメラの位置設定
	m_playerCamera.SetPlayer(SimpleMath::Vector3::Zero, m_rotate);
	// カメラの設定
	m_playerCamera.SetType(PlayerCamera::Type::TYPE_B);

	// マスクをオープンする
	auto transitionMask = GetUserResources()->GetTransitionMask();
	transitionMask->Open();
}

// 更新
void ResultScene::Update(const DX::StepTimer& timer)
{
	// キーボード取得
	auto keyState = GetUserResources()->GetKeyboardStateTracker();

	// カメラ更新
	m_playerCamera.Update((float)timer.GetElapsedSeconds());

	// シーン切り替え
	if (keyState->pressed.Enter) ChangeScene<TitleScene>();

	// PushEnterの点滅
	m_count++;
	if (m_count >= 120)	m_count -= 120;

}

// 描画
void ResultScene::Render()
{
	auto debugFont = GetUserResources()->GetDebugFont();
	debugFont->AddString(L"ResultScene", SimpleMath::Vector2(0.0f, debugFont->GetFontHeight() / 2.0f));

	// ビュー行列を設定
	m_view = SimpleMath::Matrix::CreateLookAt(
		m_playerCamera.GetEyePosition(),
		m_playerCamera.GetTargetPosition(),
		SimpleMath::Vector3::UnitY);

	// テクスチャの描画
	m_graphics->GetSpriteBatch()->Begin();
	if (m_count <= 60)
	{
		m_graphics->GetSpriteBatch()->Draw(m_pushSRV.Get(), SimpleMath::Vector2(0.0, 550.0f));
	}
	m_graphics->GetSpriteBatch()->End();

}

// 終了
void ResultScene::Finalize()
{
	m_pushSRV.Reset();
}

// デバイスに依存するリソースを作成する関数
void ResultScene::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto states = GetUserResources()->GetCommonStates();

	// グラフィックスの生成
	m_graphics->SetDeviceResources(GetUserResources()->GetDeviceResources());
	m_graphics->SetCommonStates(states);
	m_graphics->Initialize();


	// PushEnterテクスチャの読み込み
	DX::ThrowIfFailed(
		DirectX::CreateDDSTextureFromFile(device,
			L"Resources/dds/PushEnter.dds",
			nullptr,
			m_pushSRV.ReleaseAndGetAddressOf())
	);
}

// ウインドウサイズに依存するリソースを作成する関数
void ResultScene::CreateWindowSizeDependentResources()
{
	// 射影行列を作成
	RECT rect = GetUserResources()->GetDeviceResources()->GetOutputSize();
	m_proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 200.0f
	);
}

// デバイスロストした時に呼び出される関数
void ResultScene::OnDeviceLost()
{
	Finalize();
}
