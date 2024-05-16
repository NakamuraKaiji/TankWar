#include "pch.h"
#include "ResultScene.h"
#include "Utilities/Resources.h"
#include "TitleScene.h"
#include "GameObjects/SmokeEffect.h"
#include "MyLib/SoundCreate.h"

using namespace DirectX;

// コンストラクタ
ResultScene::ResultScene()
	: m_view{}
	, m_proj{}
	, m_count(0.0f)
	, m_skydomeModel{}
	, m_skydomeRotate(0.0f)
	, m_tankBodyModel{}
	, m_tankTurretModel{}
	, m_loseTankModel{}
	, m_groundModel{}
	, m_tankPosition{}
	, m_victoryFlag(false)
	, m_turretRotate(0.0f)
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

	//	勝敗に合わせた音を再生
	if (m_victoryFlag == true) m_victorySound->Play(false);
	else m_defeatSound->Play(false);

	// BGM再生
	m_bgm->Play(true);
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

	// 煙のパーティクルの更新
	m_smokeParticle->SetPosition(m_tankPosition);
	m_smokeParticle->Update((float)timer.GetElapsedSeconds());
	// 煙のビルボードを更新
	m_smokeParticle->CreateBillboard(m_playerCamera.GetTargetPosition(), m_playerCamera.GetEyePosition(), SimpleMath::Vector3::Up);

	// シーン切り替え
	if (keyState->pressed.Enter)
	{
		// マスクをオープンする
		auto transitionMask = GetUserResources()->GetTransitionMask();
		transitionMask->SetCreateMaskRequest(TransitionMask::CreateMaskRequest::NONE);
		transitionMask->Open();

		ChangeScene<TitleScene>();
	}

	// PushEnterの点滅
	m_count += (float)timer.GetElapsedSeconds();
	if (m_count >= 2.0f)	m_count = 0.0f;

	// 砲身の回転
	m_turretRotate += (float)timer.GetElapsedSeconds();

	// スカイドームの回転
	m_skydomeRotate += (float)timer.GetElapsedSeconds() * 0.05f;
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

	// インプットレイアウトを登録
	m_graphics->GetDeviceResources()->GetD3DDeviceContext()->IASetInputLayout(m_graphics->GetInputLayout());

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

	m_graphics->GetSpriteBatch()->Begin();

	// 戦車の描画
	SimpleMath::Matrix bodyWorld, turretWorld;
	bodyWorld	*= SimpleMath::Matrix::CreateRotationY(-90.0f);
	bodyWorld	*= SimpleMath::Matrix::CreateTranslation(m_tankPosition);
	turretWorld *= SimpleMath::Matrix::CreateRotationY(m_turretRotate);
	turretWorld *= bodyWorld;
	if (m_victoryFlag == true)
	{
		m_tankBodyModel->Draw(context, *states, bodyWorld, m_view, m_proj);
		m_tankTurretModel->Draw(context, *states, turretWorld, m_view, m_proj);

		// 勝利テクスチャを描画する
		m_graphics->GetSpriteBatch()->Draw(m_victoryTexture.Get(), SimpleMath::Vector2(-50.0f, 75.0f));
	}

	// 負けた戦車の描画
	if (m_victoryFlag == false)
	{
		m_loseTankModel->Draw(context, *states, bodyWorld, m_view, m_proj);

		// 敗北テクスチャを描画する
		m_graphics->GetSpriteBatch()->Draw(m_defeatTexture.Get(), SimpleMath::Vector2(-50.0f, 75.0f));

		// 煙のパーティクルの描画
		m_smokeParticle->Render();
	}



	// Pushテクスチャの描画
	if (m_count <= 1.0f)
	{
		m_graphics->GetSpriteBatch()->Draw(m_pushTexture.Get(), SimpleMath::Vector2(0.0, 550.0f));
	}

	m_graphics->GetSpriteBatch()->End();

}

// 終了
void ResultScene::Finalize()
{
	m_pushTexture.Reset();
}

// デバイスに依存するリソースを作成する関数
void ResultScene::CreateDeviceDependentResources()
{
	auto states  = GetUserResources()->GetCommonStates();

	// 勝利判定を取得する
	m_victoryFlag = GetUserResources()->GetVictoryFlag();

	// グラフィックスの生成
	m_graphics->SetDeviceResources(GetUserResources()->GetDeviceResources());
	m_graphics->SetCommonStates(states);
	m_graphics->Initialize();

	// リソースをロード
	Resources::GetInstance()->LoadResource();

	// PushEnterテクスチャを取得する
	m_pushTexture    = Resources::GetInstance()->GetPush();
	// 勝利テクスチャを取得する
	m_victoryTexture = Resources::GetInstance()->GetVictory();
	// 敗北テクスチャを取得する
	m_defeatTexture  = Resources::GetInstance()->GetDefeat();
	
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

	// パーティクルの生成
	m_smokeParticle = std::make_unique<SmokeParticle>();

	// BGMの作成
	m_bgm = SoundCreate::GetInstance()->GetSoundManager()->CreateSoundEffectInstance(
		L"Resources/Sounds/bgm.wav",
		GetUserResources()->GetAudioEngine())->CreateInstance();
	m_bgm->SetVolume(0.2f);

	// 勝利の音作成
	m_victorySound = SoundCreate::GetInstance()->GetSoundManager()->CreateSoundEffectInstance(
		L"Resources/Sounds/victory.wav",
		GetUserResources()->GetAudioEngine())->CreateInstance();
	m_victorySound->SetVolume(0.5f);

	// 敗北の音作成
	m_defeatSound = SoundCreate::GetInstance()->GetSoundManager()->CreateSoundEffectInstance(
		L"Resources/Sounds/defeat.wav",
		GetUserResources()->GetAudioEngine())->CreateInstance();
	m_defeatSound->SetVolume(0.5f);

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
