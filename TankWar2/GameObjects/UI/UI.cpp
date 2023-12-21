#include "pch.h"
#include "UI.h"
#include "Utilities/Resources.h"

// �R���X�g���N�^
UI::UI()
	: m_lifeTexture{}
	, m_life(nullptr)
{
	m_lifeTexture = Resources::GetInstance()->GetLife();
}

// ������
void UI::Initialize()
{
	// �c�@���^�X�N�o�^
	m_life = GetTaskManager()->AddTask<Life>(
		m_lifeTexture.Get(), 10, 10
		);

}