#include "pch.h"
#include "TreeRender.h"
#include "Utilities/Resources.h"
#include "GameParameter.h"

using namespace DirectX;

// コンストラクタ
TreeRender::TreeRender(ID3D11DeviceContext* context, DirectX::CommonStates* states)
	: m_context(context)
	, m_states(states)
	, m_treeModel{}
{
	// 山モデルを作成
	for (int i = 0; i < 30; i++)
	{
		m_treeModel[i] = Resources::GetInstance()->GetTree();
	}

	// 描画順の設定
	SetOt(static_cast<int>(OT_Priority::OT_BG));
}

// 描画
void TreeRender::Render()
{
	SimpleMath::Matrix treeWorld;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(50.0f, -0.5f, 30.0f));
	m_treeModel[0]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(45.0f, -0.5f, 35.0f));
	m_treeModel[1]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(60.0f, -0.5f, 40.0f));
	m_treeModel[2]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(40.0f, -0.5f, 20.0f));
	m_treeModel[3]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);

	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(0.0f, -0.5f, 60.0f));
	m_treeModel[4]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(10.0f, -0.5f, 60.0f));
	m_treeModel[5]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(5.0f, -0.5f, 50.0f));
	m_treeModel[6]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(15.0f, -0.5f, 45.0f));
	m_treeModel[7]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(15.0f, -0.5f, 60.0f));
	m_treeModel[8]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);

	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(40.0f, -0.5f, -30.0f));
	m_treeModel[9]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(35.0f, -0.5f, -45.0f));
	m_treeModel[10]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(25.0f, -0.5f, -40.0f));
	m_treeModel[11]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(35.0f, -0.5f, -20.0f));
	m_treeModel[12]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(40.0f, -0.5f, -40.0f));
	m_treeModel[13]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(37.0f, -0.5f, -32.0f));
	m_treeModel[14]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(20.0f, -0.5f, -45.0f));
	m_treeModel[15]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);

	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-40.0f, -0.5f, 20.0f));
	m_treeModel[16]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-30.0f, -0.5f, 45.0f));
	m_treeModel[17]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-30.0f, -0.5f, 35.0f));
	m_treeModel[18]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-20.0f, -0.5f, 50.0f));
	m_treeModel[19]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-40.0f, -0.5f, 25.0f));
	m_treeModel[20]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-40.0f, -0.5f, 30.0f));
	m_treeModel[21]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-25.0f, -0.5f, 45.0f));
	m_treeModel[22]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);

	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-40.0f, -0.5f, -40.0f));
	m_treeModel[23]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-20.0f, -0.5f, -45.0f));
	m_treeModel[24]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-50.0f, -0.5f, -25.0f));
	m_treeModel[25]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-50.0f, -0.5f, -35.0f));
	m_treeModel[26]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-50.0f, -0.5f, -15.0f));
	m_treeModel[27]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-45.0f, -0.5f, -45.0f));
	m_treeModel[28]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);
	treeWorld = SimpleMath::Matrix::Identity;
	treeWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-20.0f, -0.5f, -35.0f));
	m_treeModel[29]->Draw(m_context, *m_states, treeWorld, m_view, m_proj);

}

void TreeRender::SetViewProjectionMatrix(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_view = view;
	m_proj = proj;
}
