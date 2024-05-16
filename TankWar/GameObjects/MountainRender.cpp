#include "pch.h"
#include "MountainRender.h"
#include "Utilities/Resources.h"
#include "GameParameter.h"

using namespace DirectX;

// コンストラクタ
MountainRender::MountainRender(ID3D11DeviceContext* context, DirectX::CommonStates* states)
	: m_context(context)
	, m_states(states)
	, m_mountainModel{}
{
	// 山モデルを作成
	for (int i = 0; i < 5; i++)
	{
		m_mountainModel[i] = Resources::GetInstance()->GetMountain();
	}

	// 描画順の設定
	SetOt(static_cast<int>(OT_Priority::OT_BG));
}

// 描画
void MountainRender::Render()
{
	SimpleMath::Matrix mountainWorld;
	mountainWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(60.0f, -0.6f, 0.0f));
	m_mountainModel[0]->Draw(m_context, *m_states, mountainWorld, m_view, m_proj);
	mountainWorld = SimpleMath::Matrix::Identity;
	mountainWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-60.0f, -1.5f, 10.0f));
	m_mountainModel[1]->Draw(m_context, *m_states, mountainWorld, m_view, m_proj);
	mountainWorld = SimpleMath::Matrix::Identity;
	mountainWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(40.0f, -1.5f, 60.0f));
	m_mountainModel[2]->Draw(m_context, *m_states, mountainWorld, m_view, m_proj);
	mountainWorld = SimpleMath::Matrix::Identity;
	mountainWorld *= SimpleMath::Matrix::CreateRotationY(90.0f);
	mountainWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(-20.0f, -0.6f, 80.0f));
	m_mountainModel[3]->Draw(m_context, *m_states, mountainWorld, m_view, m_proj);
	mountainWorld = SimpleMath::Matrix::Identity;
	mountainWorld *= SimpleMath::Matrix::CreateRotationY(90.0f);
	mountainWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(0.0f, -0.6f, -60.0f));
	m_mountainModel[4]->Draw(m_context, *m_states, mountainWorld, m_view, m_proj);

}

void MountainRender::SetViewProjectionMatrix(DirectX::SimpleMath::Matrix view, DirectX::SimpleMath::Matrix proj)
{
	m_view = view;
	m_proj = proj;
}
