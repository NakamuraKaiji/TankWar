#include "pch.h"
#include "ResultScene.h"
#include "Utilities/Resources.h"
#include "TitleScene.h"
#include "GameObjects/SmokeEffect.h"

using namespace DirectX;

// コンストラクタ
ResultScene::ResultScene()
	: m_view{}
	, m_proj{}
	, m_count(0)
	, m_skydomeModel{}
	, m_skydomeRotate(0.0f)
	, m_tankBodyModel{}
	, m_tankTurretModel{}
	, m_loseTankModel{}
	, m_groundModel{}
	, m_tankPosition{}
	, m_smokeTime(0.0f)
	, m_vectoryFlag(false)
{
}

// 初期化
void ResultScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// カメラの位置設定
	m_playerCamera.SetPlayer(SimpleMath::Vector3::Zero, SimpleMath::Quaternion::Identity);
	// カメラの設定
	m_playerCamera.SetType(PlayerCamera::Type::TYPE_C);

	// 戦車の初期位置
	m_tankPosition = SimpleMath::Vector3(0.5f, 0.0f, -0.5f);

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

	// タスクの更新
	m_taskManager.Update((float)timer.GetElapsedSeconds());

	// シーン切り替え
	if (keyState->pressed.Enter) ChangeScene<TitleScene>();

	// PushEnterの点滅
	m_count++;
	if (m_count >= 120)	m_count -= 120;

	// スカイドームの回転
	m_skydomeRotate += timer.GetElapsedSeconds() * 0.05f;

	// デバッグ用
	if (keyState->pressed.Z)
	{
		if (m_vectoryFlag == false) m_vectoryFlag = true;
		else m_vectoryFlag = false;
	}

}

// 描画
void ResultScene::Render()
{
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	//auto debugFont = GetUserResources()->GetDebugFont();
	//debugFont->AddString(L"ResultScene", SimpleMath::Vector2(0.0f, debugFont->GetFontHeight() / 2.0f));

	// ビュー行列を設定
	m_view = SimpleMath::Matrix::CreateLookAt(
		m_playerCamera.GetEyePosition(),
		m_playerCamera.GetTargetPosition(),
		SimpleMath::Vector3::UnitY);

	// ビュー行列を設定
	m_graphics->SetViewMatrix(m_view);

	// タスクの描画
	m_taskManager.Render();

	// 地面を描画
	SimpleMath::Matrix groundWorld;
	groundWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(0.0f, -0.5f, 0.0f));
	m_groundModel->Draw(context, *states, groundWorld, m_view, m_proj);

	// スカイドームの描画
	SimpleMath::Matrix skyWorld;
	skyWorld *= SimpleMath::Matrix::CreateRotationY(m_skydomeRotate);
	m_skydomeModel->Draw(context, *states, skyWorld, m_view, m_proj);

	// 戦車の描画
	SimpleMath::Matrix world;
	world *= SimpleMath::Matrix::CreateRotationY(-90.0f);
	world *= SimpleMath::Matrix::CreateTranslation(m_tankPosition);
	if (m_vectoryFlag == true)
	{
		m_tankBodyModel->Draw(context, *states, world, m_view, m_proj);
		m_tankTurretModel->Draw(context, *states, world, m_view, m_proj);
	}

	// 負けた戦車の描画
	if (m_vectoryFlag == false)
	{
		m_loseTankModel->Draw(context, *states, world, m_view, m_proj);
	}

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
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	// グラフィックスの生成
	m_graphics->SetDeviceResources(GetUserResources()->GetDeviceResources());
	m_graphics->SetCommonStates(states);
	m_graphics->Initialize();

	// リソースをロード
	Resources::GetInstance()->LoadResource();

	// PushEnterテクスチャの読み込み
	DX::ThrowIfFailed(
		DirectX::CreateDDSTextureFromFile(device,
			L"Resources/dds/PushEnter.dds",
			nullptr,
			m_pushSRV.ReleaseAndGetAddressOf())
	);

	// スカイドームモデルの作成
	m_skydomeModel = Resources::GetInstance()->GetSkydome();
	m_skydomeModel->UpdateEffects([](IEffect* effect)
		{
			auto lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				lights->SetAmbientLightColor(Colors::Black);
				lights->SetLightEnabled(0, false);
				lights->SetLightEnabled(1, false);
				lights->SetLightEnabled(2, false);
			}
			auto basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);

	// 車体モデルの作成
	m_tankBodyModel   = Resources::GetInstance()->GetTankBody();
	// 砲身モデルの作成
	m_tankTurretModel = Resources::GetInstance()->GetTankTurret();
	// 負けた体勢モデルの作成
	m_loseTankModel   = Resources::GetInstance()->GetLoseTank();
	// 地面モデルの作成
	m_groundModel     = Resources::GetInstance()->GetGround();
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

	// 射影行列を設定
	m_graphics->SetProjectionMatrix(m_proj);
}

// デバイスロストした時に呼び出される関数
void ResultScene::OnDeviceLost()
{
	Finalize();
}
