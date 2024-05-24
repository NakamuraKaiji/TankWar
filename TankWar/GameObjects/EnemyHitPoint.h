//*********************************************************************
//			概要	：敵の体力クラス
// 
//			制作日	：5月14日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "MyLib/TaskManager.h"

class EnemyHitPoint :public Task
{
private:

	// グラフィックスへのポインタ
	Graphics* m_graphics = Graphics::GetInstance();

	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_hpTexture_a;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_hpTexture_b;

	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>> m_primitiveBatch;

	// 位置
	DirectX::SimpleMath::Vector3 m_position;

	// カメラの位置を設定
	DirectX::SimpleMath::Vector3 m_cameraPosition;

	// ビルボード
	DirectX::SimpleMath::Matrix m_billboard;

	// 体力
	float m_hp;

public:
	// コンストラクタ
	EnemyHitPoint();

	// 初期化
	void Initialize();

	// 描画
	void Render() override;

public:
	// 体力を設定
	void SetHP(float hp);
	// 体力を取得
	float GetHP() { return m_hp; }

	// 敵の位置を設定
	void SetEnemyPosition(DirectX::SimpleMath::Vector3 position);

	// カメラの位置を設定
	void SetCameraPosition(DirectX::SimpleMath::Vector3 cameraposition);
};
