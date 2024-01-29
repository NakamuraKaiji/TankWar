//*********************************************************************
//			概要	：煙のエフェクトクラス
// 
//			制作日	：12月15日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "MyLib/TaskManager.h"
#include "GameResources.h"

class SmokeEffect : public Task
{
public:

	// コンストラクタ
	SmokeEffect(DirectX::SimpleMath::Vector3 position, 
		DirectX::SimpleMath::Vector3 velocity
	);
		
	// 更新
	bool Update(float elapsedTime) override;

	// 描画
	void Render() override;

private:

	// グラフィックス
	Graphics* m_graphics = Graphics::GetInstance();

	// 位置
	DirectX::SimpleMath::Vector3 m_position;

	// 移動速度
	DirectX::SimpleMath::Vector3 m_velocity;

	// 表示時間カウンタ
	float m_timer;

	// 透明度
	float m_alpha;

	// テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_smokeTexture;
};