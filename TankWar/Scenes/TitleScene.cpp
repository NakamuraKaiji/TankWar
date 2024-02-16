#include "pch.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "Utilities/Resources.h"
#include "MyLib/SoundCreate.h"

using namespace DirectX;

// コンストラクタ
TitleScene::TitleScene()
	: m_view{}
	, m_proj{}
	, m_skydomeModel{}
	, m_tankBodyModel{}
	, m_tankTurretModel{}
	, m_bulletModel{}
	, m_groundModel{}
	, m_rotate{}
	, m_bulletPos{}
	, m_angle(-45.0f)
	, m_bulletRecovery(30.0f)
	, m_count(0.0f)
	, m_skydomeRotate(0.0f)
{
}

// 初期化
void TitleScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// カメラの位置設定
	m_playerCamera.SetPlayer(SimpleMath::Vector3::Zero, m_rotate);
	// カメラの設定
	m_playerCamera.SetType(PlayerCamera::Type::TYPE_B);
}

// 更新
void TitleScene::Update(const DX::StepTimer& timer)
{
	// キーボード取得
	auto keyState = GetUserResources()->GetKeyboardStateTracker();

	// カメラ更新
	m_playerCamera.Update((float)timer.GetElapsedSeconds());

	// シーン切り替え
	auto transitionMask = GetUserResources()->GetTransitionMask();
	if (keyState->pressed.Enter)
	{
		// 切り替え
		transitionMask->SetCreateMaskRequest(TransitionMask::CreateMaskRequest::NONE);
		transitionMask->Open();
		ChangeScene<PlayScene>();
	}

	// PushEnterの点滅
	m_count += (float)timer.GetElapsedSeconds();
	if (m_count >= 2.0f)	m_count = 0.0f;

	// 砲弾の速度の計算
	SimpleMath::Vector3 velocity = SimpleMath::Vector3::TransformNormal(BULLET_SPEED, 
								   SimpleMath::Matrix::CreateRotationY(m_angle));
	// 砲弾の移動
	m_bulletPos += velocity;

	// 砲弾回収
	if (m_bulletPos.x <= -m_bulletRecovery ||
		m_bulletPos.x >= m_bulletRecovery  ||
		m_bulletPos.y <= -m_bulletRecovery ||
		m_bulletPos.y >= m_bulletRecovery 
		)
	{
		m_bulletPos = SimpleMath::Vector3::Zero;
	}

	// スカイドームの回転
	m_skydomeRotate += (float)timer.GetElapsedSeconds() * 0.05f;
}	

// 描画
void TitleScene::Render()
{
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	// ビュー行列を設定
	m_view = SimpleMath::Matrix::CreateLookAt(
		m_playerCamera.GetEyePosition(),
		m_playerCamera.GetTargetPosition(),
		SimpleMath::Vector3::UnitY);

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
	m_rotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::Up, m_angle);
	world *= SimpleMath::Matrix::CreateFromQuaternion(m_rotate);
	world *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3::Zero);
	m_tankBodyModel->Draw(context, *states, world, m_view, m_proj);
	m_tankTurretModel->Draw(context, *states, world, m_view, m_proj);

	// 砲弾の描画
	SimpleMath::Matrix bulletWorld = SimpleMath::Matrix::Identity;
	bulletWorld *= SimpleMath::Matrix::CreateFromQuaternion(m_rotate);
	bulletWorld *= SimpleMath::Matrix::CreateTranslation(m_bulletPos);
	m_bulletModel->Draw(context, *states, bulletWorld, m_view, m_proj);

	// テクスチャの描画
	m_graphics->GetSpriteBatch()->Begin();
	m_graphics->GetSpriteBatch()->Draw(m_titleTexture.Get(), SimpleMath::Vector2(173.0f, 25.0f));
	if (m_count <= 1.0f)
	{
		m_graphics->GetSpriteBatch()->Draw(m_pushTexture.Get(), SimpleMath::Vector2(0.0, 550.0f));
	}
	m_graphics->GetSpriteBatch()->End();
}

// 終了
void TitleScene::Finalize()
{
	m_titleTexture.Reset();
	m_pushTexture.Reset();
}

// デバイスに依存するリソースを作成する関数
void TitleScene::CreateDeviceDependentResources()
{
	auto states  = GetUserResources()->GetCommonStates();

	// グラフィックスの生成
	m_graphics->SetDeviceResources(GetUserResources()->GetDeviceResources());
	m_graphics->SetCommonStates(states);
	m_graphics->Initialize();

	// リソースをロード
	Resources::GetInstance()->LoadResource();

	// タイトルテクスチャを取得する
	m_titleTexture = Resources::GetInstance()->GetTitle();
	// PushEnterテクスチャを取得する
	m_pushTexture  = Resources::GetInstance()->GetPush();

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
	// 砲弾モデルの作成
	m_bulletModel     = Resources::GetInstance()->GetBullet();
	// 地面モデルの作成
	m_groundModel     = Resources::GetInstance()->GetGround();
}

// ウインドウサイズに依存するリソースを作成する関数
void TitleScene::CreateWindowSizeDependentResources()
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
void TitleScene::OnDeviceLost()
{
	Finalize();
}
