#include "pch.h"
#include "Life.h"

using namespace DirectX;

// コストラクタ
Life::Life(ID3D11ShaderResourceView* pTexture, int x, int y)
	: m_pTexture(pTexture)
	, m_x(x)
	, m_y(y)
	, m_life(3)
{
}

// 描画
void Life::Render()
{
	for (int i = 0; i < m_life; i++)
	{
		m_graphics->GetSpriteBatch()->Draw(m_pTexture,
			SimpleMath::Vector2((float)m_x + (i * 110), (float)m_y)
		);
	}
}
