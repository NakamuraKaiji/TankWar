//*********************************************************************
//			概要	：タイトルシーンクラス
// 
//			制作日	：11月29日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "MyLib/SceneManager.h"
#include "UserResources.h"
#include "GameObjects/PlayerCamera.h"
#include "GameObjects/GameParameter.h"

class TitleScene : public Scene<UserResources>
{
public:

	// コストラクタ
	TitleScene();

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

	// タイトルテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_titleTexture;

	// PushEnterテクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pushTexture;

	// グラフィックス
	Graphics* m_graphics = Graphics::GetInstance();

	// スカイドームモデルへのポインタ
	std::shared_ptr<DirectX::Model> m_skydomeModel;

	// 車体モデルへのポインタ
	std::shared_ptr<DirectX::Model> m_tankBodyModel;

	// 砲身モデルへのポインタ
	std::shared_ptr<DirectX::Model> m_tankTurretModel;

	// 砲弾モデルへのポインタ
	std::shared_ptr<DirectX::Model> m_bulletModel;

	// 地面モデルへのポインタ
	std::shared_ptr<DirectX::Model> m_groundModel;

	// カメラ
	PlayerCamera m_playerCamera;

private:

	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;

	// 射影行列
	DirectX::SimpleMath::Matrix m_proj;

	// 角度
	DirectX::SimpleMath::Quaternion m_rotate;

	// カウント
	float m_count;

	// 砲弾の位置
	DirectX::SimpleMath::Vector3 m_bulletPos;

	// 砲弾の角度
	float m_angle;

	// 砲弾回収
	float m_bulletRecovery;

	// スカイドームの回転
	float m_skydomeRotate;
};