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
#include "ImaseLib/TaskManager.h"
#include "ImaseLib/GridFloor.h"
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

	// タスクマネージャー
	Imase::TaskManager m_taskManager;

	// グリッドの床へのポインタ
	std::unique_ptr<Imase::GridFloor> m_gridFloor;

	// スカイドームモデルへのポインタ
	std::shared_ptr<DirectX::Model> m_skydomeModel;

	// 車体モデルへのポインタ
	std::shared_ptr<DirectX::Model> m_tankBodyModel;

	// 砲身モデルへのポインタ
	std::shared_ptr<DirectX::Model> m_tankTurretModel;

	// 負けた体勢の戦車モデルへのポインタ
	std::shared_ptr<DirectX::Model> m_loseTankModel;

private:

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// 射影行列
	DirectX::SimpleMath::Matrix m_proj;

	// カウント
	int m_count;

	// スカイドームの回転
	float m_skydomeRotate;

	// 車体の位置
	DirectX::SimpleMath::Vector3 m_tankPosition;

	// 煙の出る間隔
	float m_smokeTime;

	// 勝ったか負けたか
	bool m_vectoryFlag;

};