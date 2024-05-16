//*********************************************************************
//			概要	：木クラス
// 
//			制作日	：5月8日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "MyLib/TaskManager.h"

class TreeRender : public Task
{
private:

	// デバイスコンテキストへのポインタ
	ID3D11DeviceContext* m_context;

	// コモンステートへのポインタ
	DirectX::CommonStates* m_states;

	DirectX::SimpleMath::Matrix m_view, m_proj;

	// グラフィックスへのポインタ
	Graphics* m_graphics = Graphics::GetInstance();

	// 山モデル
	std::shared_ptr<DirectX::Model> m_treeModel[30];

public:
	// コンストラクタ
	TreeRender(ID3D11DeviceContext* context,
		DirectX::CommonStates* states
	);

	// 描画
	void Render() override;

	void SetViewProjectionMatrix(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj);
};