#include "pch.h"
#include "UI.h"
#include "Utilities/Resources.h"

// コンストラクタ
UI::UI()
	: m_lifeTexture{}
	, m_life(nullptr)
{
	m_lifeTexture = Resources::GetInstance()->GetLife();
}

// 初期化
void UI::Initialize()
{
	// 残機数タスク登録
	m_life = GetTaskManager()->AddTask<Life>(
		m_lifeTexture.Get(), 10, 10
		);

}