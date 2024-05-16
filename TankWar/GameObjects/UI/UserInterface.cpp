#include "pch.h"
#include "UserInterface.h"
#include "Utilities/Resources.h"

// コンストラクタ
UserInterface::UserInterface()
	: m_lifeTexture{}
	, m_life(nullptr)
{
	// 画像を取得する
	m_lifeTexture = Resources::GetInstance()->GetLife();
	m_texture1    = Resources::GetInstance()->GetNumber1();
	m_texture2    = Resources::GetInstance()->GetNumber2();
	m_texture3    = Resources::GetInstance()->GetNumber3();
}

// 初期化
void UserInterface::Initialize()
{
	// 残機数のタスク登録
	m_life = GetTaskManager()->AddTask<Life>(10, 10);
	m_life->SetTexture(m_lifeTexture.Get());

	// スタートカウントのタスク登録
	m_startCount = GetTaskManager()->AddTask<StartCount>(256, 400, SCREEN_W, SCREEN_H);
	m_startCount->SetTexture1(m_texture1.Get());
	m_startCount->SetTexture2(m_texture2.Get());
	m_startCount->SetTexture3(m_texture3.Get());
}