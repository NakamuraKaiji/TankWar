//*********************************************************************
//			概要	：リザルトシーンクラス
// 
//			制作日	：1月9日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "MyLib/SceneManager.h"
#include "UserResources.h"
#include "MyLib/TaskManager.h"
#include "GameObjects/PlayerCamera.h"
#include "GameObjects/GameParameter.h"
#include "GameObjects/SmokeParticle.h"

class ResultScene : public Scene<UserResources>
{
public:

	// コンストラクタ
	ResultScene();

	// 初期化
	void Initialize() override;

	// 更新
	void Update(const DX::StepTimer& timer) override;

	// 描画
	void Render() override;

	// 終了
	void Finalize() override;

	// デバイスに依存するリソースを作成する関数
	void CreateDeviceDependentResources() override;

	// ウインドウサイズに依存するリソースを作成する関数
	void CreateWindowSizeDependentResources() override;

	// デバイスロストした時に呼び出される関数
	void OnDeviceLost() override;

private:
	// グラフィックス
	Graphics* m_graphics = Graphics::GetInstance();

	// PushEnterテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pushTexture;

	// 勝利テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_victoryTexture;

	// 敗北テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_defeatTexture;

	// カメラ
	PlayerCamera m_playerCamera;

	// タスクマネージャー
	TaskManager m_taskManager;

	// スカイドームモデルへのポインタ
	std::shared_ptr<DirectX::Model> m_skydomeModel;
	// 車体モデルへのポインタ
	std::shared_ptr<DirectX::Model> m_tankBodyModel;
	// 砲身モデルへのポインタ
	std::shared_ptr<DirectX::Model> m_tankTurretModel;
	// 負けた体勢の戦車モデルへのポインタ
	std::shared_ptr<DirectX::Model> m_loseTankModel;
	// 地面モデルへのポインタ
	std::shared_ptr<DirectX::Model> m_groundModel;

	// 煙のパーティクル
	std::unique_ptr<SmokeParticle> m_smokeParticle;

	// BGM
	std::unique_ptr<DirectX::SoundEffectInstance> m_bgm;
	// 勝利の音
	std::unique_ptr<DirectX::SoundEffectInstance> m_victorySound;
	// 敗北の音
	std::unique_ptr<DirectX::SoundEffectInstance> m_defeatSound;


private:

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// 射影行列
	DirectX::SimpleMath::Matrix m_proj;

	// カウント
	float m_count;

	// 砲身の回転
	float m_turretRotate;

	// スカイドームの回転
	float m_skydomeRotate;

	// 車体の位置
	DirectX::SimpleMath::Vector3 m_tankPosition;

	// 勝利判定
	bool m_victoryFlag;

};