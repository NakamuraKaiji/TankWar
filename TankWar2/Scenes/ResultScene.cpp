#include "pch.h"
#include "ResultScene.h"
#include "Utilities/Resources.h"
#include "TitleScene.h"

using namespace DirectX;

// �R���X�g���N�^
ResultScene::ResultScene()
	: m_view{}
	, m_proj{}
	, m_count(0)
	, m_rotate{}
{
}

// ������
void ResultScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// �J�����̈ʒu�ݒ�
	m_playerCamera.SetPlayer(SimpleMath::Vector3::Zero, m_rotate);
	// �J�����̐ݒ�
	m_playerCamera.SetType(PlayerCamera::Type::TYPE_B);

	// �}�X�N���I�[�v������
	auto transitionMask = GetUserResources()->GetTransitionMask();
	transitionMask->Open();
}

// �X�V
void ResultScene::Update(const DX::StepTimer& timer)
{
	// �L�[�{�[�h�擾
	auto keyState = GetUserResources()->GetKeyboardStateTracker();

	// �J�����X�V
	m_playerCamera.Update((float)timer.GetElapsedSeconds());

	// �V�[���؂�ւ�
	if (keyState->pressed.Enter) ChangeScene<TitleScene>();

	// PushEnter�̓_��
	m_count++;
	if (m_count >= 120)	m_count -= 120;

}

// �`��
void ResultScene::Render()
{
	auto debugFont = GetUserResources()->GetDebugFont();
	debugFont->AddString(L"ResultScene", SimpleMath::Vector2(0.0f, debugFont->GetFontHeight() / 2.0f));

	// �r���[�s���ݒ�
	m_view = SimpleMath::Matrix::CreateLookAt(
		m_playerCamera.GetEyePosition(),
		m_playerCamera.GetTargetPosition(),
		SimpleMath::Vector3::UnitY);

	// �e�N�X�`���̕`��
	m_graphics->GetSpriteBatch()->Begin();
	if (m_count <= 60)
	{
		m_graphics->GetSpriteBatch()->Draw(m_pushSRV.Get(), SimpleMath::Vector2(0.0, 550.0f));
	}
	m_graphics->GetSpriteBatch()->End();

}

// �I��
void ResultScene::Finalize()
{
	m_pushSRV.Reset();
}

// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬����֐�
void ResultScene::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto states = GetUserResources()->GetCommonStates();

	// �O���t�B�b�N�X�̐���
	m_graphics->SetDeviceResources(GetUserResources()->GetDeviceResources());
	m_graphics->SetCommonStates(states);
	m_graphics->Initialize();


	// PushEnter�e�N�X�`���̓ǂݍ���
	DX::ThrowIfFailed(
		DirectX::CreateDDSTextureFromFile(device,
			L"Resources/dds/PushEnter.dds",
			nullptr,
			m_pushSRV.ReleaseAndGetAddressOf())
	);
}

// �E�C���h�E�T�C�Y�Ɉˑ����郊�\�[�X���쐬����֐�
void ResultScene::CreateWindowSizeDependentResources()
{
	// �ˉe�s����쐬
	RECT rect = GetUserResources()->GetDeviceResources()->GetOutputSize();
	m_proj = SimpleMath::Matrix::CreatePerspectiveFieldOfView(
		XMConvertToRadians(45.0f),
		static_cast<float>(rect.right) / static_cast<float>(rect.bottom),
		0.1f, 200.0f
	);
}

// �f�o�C�X���X�g�������ɌĂяo�����֐�
void ResultScene::OnDeviceLost()
{
	Finalize();
}
