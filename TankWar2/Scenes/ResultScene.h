//*********************************************************************
//			概要	：リザルトシーンクラス
// 
//			制作日	：1月9日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "ImaseLib/SceneManager.h"
#include "UserResources.h"
#include "GameObjects/PlayerCamera.h"
#include "GameObjects/GameParameter.h"

class ResultScene : public Imase::Scene<UserResources>
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

	// PushEnterテクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pushSRV;

	// カメラ
	PlayerCamera m_playerCamera;


private:

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// 射影行列
	DirectX::SimpleMath::Matrix m_proj;

	// カウント
	int m_count;

	// 角度
	DirectX::SimpleMath::Quaternion m_rotate;


};