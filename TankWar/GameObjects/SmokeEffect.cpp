#include "pch.h"
#include "SmokeEffect.h"
#include "GameParameter.h"
#include "Utilities/Resources.h"

using namespace DirectX;

// コンストラクタ
SmokeEffect::SmokeEffect(DirectX::SimpleMath::Vector3 position, 
	DirectX::SimpleMath::Vector3 velocity
)
	:  m_position(position)
	, m_velocity(velocity)
	, m_timer(WHITE_SMOKE_DISPLAY_TIME)
	, m_alpha(0.0f)
{
	// テクスチャを取得
	m_smokeTexture = Resources::GetInstance()->GetParticle();

	// 描画順の設定
	SetOt(static_cast<int>(OT_Priority::OT_Top));
}

// 更新
bool SmokeEffect::Update(float elapsedTime)
{
	// 時間になったらタスクは消える
	m_timer -= elapsedTime;
	if (m_timer <= 0.0f) return false;

	// 徐々に消えていく
	m_alpha = m_timer / WHITE_SMOKE_DISPLAY_TIME;

	m_position += m_velocity;

	// 速度を落としていく
	float speed = m_velocity.Length();
	speed *= WHITE_SMOKE_FRICTION;
	m_velocity.Normalize();
	m_velocity *= speed;

	return true;
}

// 描画
void SmokeEffect::Render()
{
	// 頂点情報
	VertexPositionColorTexture vertex[4] =
	{
		VertexPositionColorTexture(XMFLOAT3( 0.5f, 0.0f,  0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, m_alpha), XMFLOAT2(0.0f, 0.0f)),
		VertexPositionColorTexture(XMFLOAT3(-0.5f, 0.0f,  0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, m_alpha), XMFLOAT2(1.0f, 0.0f)),
		VertexPositionColorTexture(XMFLOAT3(-0.5f, 0.0f, -0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, m_alpha), XMFLOAT2(1.0f, 1.0f)),
		VertexPositionColorTexture(XMFLOAT3( 0.5f, 0.0f, -0.5f), XMFLOAT4(1.0f, 1.0f, 1.0f, m_alpha), XMFLOAT2(0.0f, 1.0f)),
	};

	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_graphics->GetCommonStates();
	auto effect = m_graphics->GetBasicEffect();

	context->OMSetBlendState(states->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	context->OMSetDepthStencilState(states->DepthRead(), 0);
	context->RSSetState(states->CullCounterClockwise());

	// 設定
	float scale = 2.0f * (1.0f - m_timer / WHITE_SMOKE_DISPLAY_TIME);
	SimpleMath::Matrix world = SimpleMath::Matrix::CreateScale(scale) * SimpleMath::Matrix::CreateTranslation(m_position);
	effect->SetWorld(world);
	effect->SetTexture(m_smokeTexture.Get());

	// 表示
	m_graphics->DrawPrimitiveBegin(m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());
	m_graphics->GetPrimitiveBatch()->DrawQuad(vertex[0], vertex[1], vertex[2], vertex[3]);
	m_graphics->DrawPrimitiveEnd();
}
