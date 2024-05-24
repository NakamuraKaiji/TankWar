#include "pch.h"
#include "PlayScene.h"
#include "Utilities/Resources.h"
#include "GameObjects/GameResources.h"
#include "MyLib/SoundCreate.h"
#include "ResultScene.h"

using namespace DirectX;

// コンストラクタ
PlayScene::PlayScene()
	: m_view{}
	, m_proj{}
	, m_skydomeModel{}
	, m_skydomeRotate(0.0f)
	, m_moutainRender{}
	, m_treeRender{}
	, m_stage{}
	, m_userInterface{}
	, m_life(0)
	, m_ratio(1.0)
	, m_explosionPlayerFlag(false)
	, m_explosionEnemyFlag(false)
	, m_explosionPlayerTime(0.0f)
	, m_explosionEnemyTime(0.0f)
	, m_countSoundFlag(false)
{
}

// 初期化
void PlayScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// 残機数を設定
	m_life = PLAYER_CNT;
	m_userInterface->GetLife()->SetLife(m_life);

	// 敵のHPを設定
	m_ratio = ENEMY_HP;

	// BGM再生
	m_bgm->Play(true);

	// スタートまでの時間を設定
	m_userInterface->GetStartCount()->SetCount(START_COUNT);
}

// 更新
void PlayScene::Update(const DX::StepTimer& timer)
{
	auto kbTracker = GetUserResources()->GetKeyboardStateTracker();

	// カメラの設定
	m_playerCamera.SetType(PlayerCamera::Type::TYPE_A);
	m_playerCamera.SetPlayer(m_stage->GetPlayer()->GetPosition(), m_stage->GetPlayer()->GetBodyRotate() * m_stage->GetPlayer()->GetTurretRotate());

	// カメラの更新
	m_playerCamera.Update((float)timer.GetElapsedSeconds());

	// 山モデルにviewとprojを設定する
	m_moutainRender->SetViewProjectionMatrix(m_view, m_proj);
	// 木モデルにviewとprojを設定する
	m_treeRender->SetViewProjectionMatrix(m_view, m_proj);

	// カメラの情報をHPに渡す
	m_stage->GetEnemy()->GetEnemyHitPoint()->SetCameraPosition(m_playerCamera.GetEyePosition());

	// スカイドームの回転
	m_skydomeRotate += (float)timer.GetElapsedSeconds() * 0.05f;

	// カウントを戦車に渡す
	m_stage->GetPlayer()->SetStartFlag(m_userInterface->GetStartCount()->GetStartFlag());
	m_stage->GetEnemy()->SetStartFlag(m_userInterface->GetStartCount()->GetStartFlag());

	// カウント毎に音を鳴らす
	if (!m_userInterface->GetStartCount()->GetStartFlag())
	{
		float count = m_userInterface->GetStartCount()->GetCount();
		if (count <= 3.0f && count > 2.9f) m_countSoundFlag = true;
		if (count <= 2.1f && count > 2.0f) m_countSoundFlag = true;
		if (count <= 1.1f && count > 1.0f) m_countSoundFlag = true;
		if (m_countSoundFlag)
		{
			m_countSound->Stop();
			m_countSound->Play();
			m_countSoundFlag = false;
		}
		if (count <= 0.02f)
		{
			m_explosionSound->Stop();
			m_explosionSound->Play();
		}
	}

	// タスクの更新
	m_taskManager.Update((float)timer.GetElapsedSeconds());

	// プレイヤーの移動処理
	m_stage->GetPlayer()->Move(kbTracker, (float)timer.GetElapsedSeconds());

	// 衝突判定の更新
	m_collisionManager.Update();

	// 砲弾を発射したら音を鳴らす
	if (m_stage->GetPlayer()->GetShotFlag() || m_stage->GetEnemy()->GetShotFlag())
	{
		m_shotSound->Stop();
		m_shotSound->Play();
	}

	// 当たったら残機を減らす
	if (m_stage->GetPlayer()->GetState() == PlayerTank::PlayerState::Hit)
	{
		m_life--;
		m_userInterface->GetLife()->SetLife(m_life);

		// 爆発音を鳴らす
		m_explosionSound->Stop();
		m_explosionSound->Play();

		// 爆発のパーティクルの位置の設定
		m_explosionParticle[0]->SetPosition(m_stage->GetPlayer()->GetPosition());
		m_explosionPlayerFlag = true;

		if (m_life == 0)
		{
			// マスクをオープンする
			auto transitionMask = GetUserResources()->GetTransitionMask();
			transitionMask->SetCreateMaskRequest(TransitionMask::CreateMaskRequest::NONE);
			transitionMask->Open();

			// リザルトシーンに切り替え
			ChangeScene<ResultScene>();
			GetUserResources()->SetVictoryFlag(false);
		}
	}

	// 敵が砲弾に当たったらHPを減らす
	if (m_stage->GetEnemy()->GetState() == EnemyTank::EnemyState::Hit)
	{
		m_ratio = m_ratio - ENEMY_REDUCE_HP;
		m_stage->GetEnemy()->GetEnemyHitPoint()->SetHP(m_ratio);

		// 爆発音を鳴らす
		m_explosionSound->Stop();
		m_explosionSound->Play();

		// 爆発のパーティクルの位置を設定
		m_explosionParticle[1]->SetPosition(m_stage->GetEnemy()->GetPosition());
		m_explosionEnemyFlag = true;

		if (m_ratio <= 0.0f)
		{
			// マスクをオープンする
			auto transitionMask = GetUserResources()->GetTransitionMask();
			transitionMask->SetCreateMaskRequest(TransitionMask::CreateMaskRequest::NONE);
			transitionMask->Open();

			// リザルトシーンに切り替え
			ChangeScene<ResultScene>();
			GetUserResources()->SetVictoryFlag(true);
		}
	}

	// 0.4秒経過したら消す
	if (m_explosionPlayerFlag == true)
	{
		m_explosionPlayerTime += (float)timer.GetElapsedSeconds();
		m_explosionParticle[0]->SetScale(SimpleMath::Vector3::One);
		if (m_explosionPlayerTime >= EXPLOSION_DISPLAY_TIME)
		{
			m_explosionPlayerFlag = false;
			m_explosionPlayerTime = 0.0f;
		}
	}
	if (m_explosionEnemyFlag == true)
	{
		m_explosionEnemyTime += (float)timer.GetElapsedSeconds();
		m_explosionParticle[1]->SetScale(SimpleMath::Vector3::One);
		if (m_explosionEnemyTime >= EXPLOSION_DISPLAY_TIME)
		{
			m_explosionEnemyFlag = false;
			m_explosionEnemyTime = 0.0f;
		}
	}
	// スケールの設定
	if (m_explosionPlayerFlag == false) m_explosionParticle[0]->SetScale(SimpleMath::Vector3::Zero);
	if (m_explosionEnemyFlag == false) m_explosionParticle[1]->SetScale(SimpleMath::Vector3::Zero);

	// 爆発の更新
	m_explosionParticle[0]->Update((float)timer.GetElapsedSeconds());
	m_explosionParticle[1]->Update((float)timer.GetElapsedSeconds());

}

// 描画
void PlayScene::Render()
{
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	m_view = SimpleMath::Matrix::CreateLookAt(
		m_playerCamera.GetEyePosition(),
		m_playerCamera.GetTargetPosition(),
		SimpleMath::Vector3::UnitY
	);

	// ビュー行列を設定
	m_graphics->SetViewMatrix(m_view);

	// インプットレイアウトを登録
	m_graphics->GetDeviceResources()->GetD3DDeviceContext()->IASetInputLayout(m_graphics->GetInputLayout());

	// 地面を描画
	SimpleMath::Matrix groundWorld;
	groundWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(0.0f, -0.5f, 0.0f));
	m_groundModel->Draw(context, *states, groundWorld, m_view, m_proj);

	// スカイドームの描画
	SimpleMath::Matrix skyWorld;
	skyWorld *= SimpleMath::Matrix::CreateRotationY(m_skydomeRotate);
	skyWorld *= SimpleMath::Matrix::CreateTranslation(m_stage->GetPlayer()->GetPosition());
	m_skydomeModel->Draw(context, *states, skyWorld, m_view, m_proj);

	if (m_graphics->GetSpriteBatch()) m_graphics->GetSpriteBatch()->Begin();

	// タスクの描画
	m_taskManager.Render();

	if (m_graphics->GetSpriteBatch()) m_graphics->GetSpriteBatch()->End();

	// 爆発のパーティクルの描画
	if (m_explosionPlayerFlag == true)
	{
		m_explosionParticle[0]->CreateBillboard(m_stage->GetEnemy()->GetPosition(), m_playerCamera.GetEyePosition(), SimpleMath::Vector3::Up);
		m_explosionParticle[0]->Render();
	}
	if (m_explosionEnemyFlag == true)
	{
		m_explosionParticle[1]->CreateBillboard(m_playerCamera.GetTargetPosition(), m_playerCamera.GetEyePosition(), SimpleMath::Vector3::Up);
		m_explosionParticle[1]->Render();
	}
}

// 終了
void PlayScene::Finalize()
{
}

// デバイスに依存するリソースを作成する関数
void PlayScene::CreateDeviceDependentResources()
{
	auto states = GetUserResources()->GetCommonStates();

	// グラフィックスの生成
	m_graphics->GetInstance()->SetDeviceResources(GetUserResources()->GetDeviceResources());
	m_graphics->GetInstance()->SetCommonStates(states);
	m_graphics->Initialize();

	// リソースをロード
	Resources::GetInstance()->LoadResource();

	// 地面モデルを作成
	m_groundModel   = Resources::GetInstance()->GetGround();
	// スカイドームを作成
	m_skydomeModel  = Resources::GetInstance()->GetSkydome();

	// 山モデルを作成
	if (m_moutainRender) m_moutainRender->Kill();
	m_moutainRender = m_taskManager.AddTask<MountainRender>(
		m_graphics->GetDeviceResources()->GetD3DDeviceContext(),
		m_graphics->GetCommonStates());

	// 木モデルを作成
	if (m_treeRender) m_treeRender->Kill();
	m_treeRender = m_taskManager.AddTask<TreeRender>(
		m_graphics->GetDeviceResources()->GetD3DDeviceContext(),
		m_graphics->GetCommonStates());

	// ステージを作成 
	GameResources gameResources = {&m_collisionManager};
	if (m_stage) m_stage->Kill();
	m_stage = m_taskManager.AddTask<Stage>(gameResources);
	
	// ステージデータの設定
	m_stage->SetStageData();

	// UIの作成
	if (m_userInterface) m_userInterface->Kill();
	m_userInterface = m_taskManager.AddTask<UserInterface>();

	// 爆発のパーティクル生成
	for (int i = 0; i < 2; i++)
	{
		m_explosionParticle[i] = std::make_unique<ExplosionParticle>();
	}

	// BGMの作成
	m_bgm = SoundCreate::GetInstance()->GetSoundManager()->CreateSoundEffectInstance(
		L"Resources/Sounds/bgm.wav",
		GetUserResources()->GetAudioEngine())->CreateInstance();
	m_bgm->SetVolume(0.3f);

	// 爆発音の作成
	m_explosionSound = SoundCreate::GetInstance()->GetSoundManager()->CreateSoundEffectInstance(
		L"Resources/Sounds/explosion.wav",
		GetUserResources()->GetAudioEngine())->CreateInstance();
	m_explosionSound->SetVolume(0.6f);

	// 発射音の作成
	m_shotSound = SoundCreate::GetInstance()->GetSoundManager()->CreateSoundEffectInstance(
		L"Resources/Sounds/shot.wav",
		GetUserResources()->GetAudioEngine())->CreateInstance();
	m_shotSound->SetVolume(0.8f);

	// カウントオンの作成
	m_countSound = SoundCreate::GetInstance()->GetSoundManager()->CreateSoundEffectInstance(
		L"Resources/Sounds/Count.wav",
		GetUserResources()->GetAudioEngine())->CreateInstance();
	m_countSound->SetVolume(1.0f);
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
