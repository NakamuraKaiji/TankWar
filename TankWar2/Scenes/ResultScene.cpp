#include "pch.h"
#include "ResultScene.h"
#include "Utilities/Resources.h"
#include "TitleScene.h"
#include "GameObjects/SmokeEffect.h"

using namespace DirectX;

// �R���X�g���N�^
ResultScene::ResultScene()
	: m_view{}
	, m_proj{}
	, m_count(0)
	, m_skydomeModel{}
	, m_skydomeRotate(0.0f)
	, m_tankBodyModel{}
	, m_tankTurretModel{}
	, m_loseTankModel{}
	, m_groundModel{}
	, m_tankPosition{}
	, m_smokeTime(0.0f)
	, m_vectoryFlag(false)
{
}

// ������
void ResultScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// �J�����̈ʒu�ݒ�
	m_playerCamera.SetPlayer(SimpleMath::Vector3::Zero, SimpleMath::Quaternion::Identity);
	// �J�����̐ݒ�
	m_playerCamera.SetType(PlayerCamera::Type::TYPE_C);

	// ��Ԃ̏����ʒu
	m_tankPosition = SimpleMath::Vector3(0.5f, 0.0f, -0.5f);

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

	// �^�X�N�̍X�V
	m_taskManager.Update((float)timer.GetElapsedSeconds());

	// �V�[���؂�ւ�
	if (keyState->pressed.Enter) ChangeScene<TitleScene>();

	// PushEnter�̓_��
	m_count++;
	if (m_count >= 120)	m_count -= 120;

	// �X�J�C�h�[���̉�]
	m_skydomeRotate += timer.GetElapsedSeconds() * 0.05f;

	// �f�o�b�O�p
	if (keyState->pressed.Z)
	{
		if (m_vectoryFlag == false) m_vectoryFlag = true;
		else m_vectoryFlag = false;
	}

}

// �`��
void ResultScene::Render()
{
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	//auto debugFont = GetUserResources()->GetDebugFont();
	//debugFont->AddString(L"ResultScene", SimpleMath::Vector2(0.0f, debugFont->GetFontHeight() / 2.0f));

	// �r���[�s���ݒ�
	m_view = SimpleMath::Matrix::CreateLookAt(
		m_playerCamera.GetEyePosition(),
		m_playerCamera.GetTargetPosition(),
		SimpleMath::Vector3::UnitY);

	// �r���[�s���ݒ�
	m_graphics->SetViewMatrix(m_view);

	// �^�X�N�̕`��
	m_taskManager.Render();

	// �n�ʂ�`��
	SimpleMath::Matrix groundWorld;
	groundWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(0.0f, -0.5f, 0.0f));
	m_groundModel->Draw(context, *states, groundWorld, m_view, m_proj);

	// �X�J�C�h�[���̕`��
	SimpleMath::Matrix skyWorld;
	skyWorld *= SimpleMath::Matrix::CreateRotationY(m_skydomeRotate);
	m_skydomeModel->Draw(context, *states, skyWorld, m_view, m_proj);

	// ��Ԃ̕`��
	SimpleMath::Matrix world;
	world *= SimpleMath::Matrix::CreateRotationY(-90.0f);
	world *= SimpleMath::Matrix::CreateTranslation(m_tankPosition);
	if (m_vectoryFlag == true)
	{
		m_tankBodyModel->Draw(context, *states, world, m_view, m_proj);
		m_tankTurretModel->Draw(context, *states, world, m_view, m_proj);
	}

	// ��������Ԃ̕`��
	if (m_vectoryFlag == false)
	{
		m_loseTankModel->Draw(context, *states, world, m_view, m_proj);
	}

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
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	// �O���t�B�b�N�X�̐���
	m_graphics->SetDeviceResources(GetUserResources()->GetDeviceResources());
	m_graphics->SetCommonStates(states);
	m_graphics->Initialize();

	// ���\�[�X�����[�h
	Resources::GetInstance()->LoadResource();

	// PushEnter�e�N�X�`���̓ǂݍ���
	DX::ThrowIfFailed(
		DirectX::CreateDDSTextureFromFile(device,
			L"Resources/dds/PushEnter.dds",
			nullptr,
			m_pushSRV.ReleaseAndGetAddressOf())
	);

	// �X�J�C�h�[�����f���̍쐬
	m_skydomeModel = Resources::GetInstance()->GetSkydome();
	m_skydomeModel->UpdateEffects([](IEffect* effect)
		{
			auto lights = dynamic_cast<IEffectLights*>(effect);
			if (lights)
			{
				lights->SetAmbientLightColor(Colors::Black);
				lights->SetLightEnabled(0, false);
				lights->SetLightEnabled(1, false);
				lights->SetLightEnabled(2, false);
			}
			auto basicEffect = dynamic_cast<BasicEffect*>(effect);
			if (basicEffect)
			{
				basicEffect->SetEmissiveColor(Colors::White);
			}
		}
	);

	// �ԑ̃��f���̍쐬
	m_tankBodyModel   = Resources::GetInstance()->GetTankBody();
	// �C�g���f���̍쐬
	m_tankTurretModel = Resources::GetInstance()->GetTankTurret();
	// �������̐����f���̍쐬
	m_loseTankModel   = Resources::GetInstance()->GetLoseTank();
	// �n�ʃ��f���̍쐬
	m_groundModel     = Resources::GetInstance()->GetGround();
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

	// �ˉe�s���ݒ�
	m_graphics->SetProjectionMatrix(m_proj);
}

// �f�o�C�X���X�g�������ɌĂяo�����֐�
void ResultScene::OnDeviceLost()
{
	Finalize();
}
