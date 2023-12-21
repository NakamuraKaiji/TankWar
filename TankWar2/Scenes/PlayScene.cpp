//*********************************************************************
//			�T�v	�F�v���C�V�[���N���X
// 
//			�����	�F10��3��
// 
//			�����	�FKaiji Nakamura
//*********************************************************************
#include "pch.h"
#include "PlayScene.h"
#include "Utilities/Resources.h"
#include "GameObjects/GameResources.h"

using namespace DirectX;

// �R���X�g���N�^
PlayScene::PlayScene()
	: m_view{}
	, m_proj{}
	, m_skydomeModel{}
	, m_stage{}
	, m_UI{}
	, m_life{}
{
}

// ������
void PlayScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// �c�@����ݒ�
	m_life = PLAYER_CNT;
	m_UI->GetLife()->SetLife(m_life);
}

// �X�V
void PlayScene::Update(const DX::StepTimer& timer)
{
	auto kbTracker = GetUserResources()->GetKeyboardStateTracker();

	// �^�X�N�̍X�V
	m_taskManager.Update(timer.GetElapsedSeconds());

	// �S�Ă̓G�𓮂���
	m_stage->StopAllEnemy(false);

	// �v���C���[�̈ړ�����
	m_stage->GetPlayer()->Move(kbTracker);

	// �J�����̐ݒ�
	m_playerCamera.SetType(PlayerCamera::Type::TYPE_A);
	m_playerCamera.SetPlayer(m_stage->GetPlayer()->GetPosition(), m_stage->GetPlayer()->GetBodyRotate() * m_stage->GetPlayer()->GetTurretRotate());

	// �J�����̍X�V
	m_playerCamera.Update(timer.GetElapsedSeconds());
	
	// �Փ˔���̍X�V
	m_collisionManager.Update();

	if (kbTracker->pressed.Z)
	{
		m_life--;
		m_UI->GetLife()->SetLife(m_life);
	}
}

// �`��
void PlayScene::Render()
{
	auto debugFont = GetUserResources()->GetDebugFont();
	//debugFont->AddString(L"PlayScene", SimpleMath::Vector2(0.0f, debugFont->GetFontHeight()));

	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	m_view = SimpleMath::Matrix::CreateLookAt(
		m_playerCamera.GetEyePosition(),
		m_playerCamera.GetTargetPosition(),
		SimpleMath::Vector3::UnitY
	);

	// �r���[�s���ݒ�
	m_graphics->SetViewMatrix(m_view);

	// �X�J�C�h�[���̕`��
	SimpleMath::Matrix skyWorld;
	skyWorld = SimpleMath::Matrix::CreateTranslation(m_stage->GetPlayer()->GetPosition());
	m_skydomeModel->Draw(context, *states, skyWorld, m_view, m_proj);

	// �O���b�h�̏���`��
	m_gridFloor->Render(context, m_view = m_graphics->GetViewMatrix(), m_graphics->GetProjectionMatrix());

	if (m_graphics->GetSpriteBatch()) m_graphics->GetSpriteBatch()->Begin();

	// �^�X�N�̕`��
	m_taskManager.Render();

	if (m_graphics->GetSpriteBatch()) m_graphics->GetSpriteBatch()->End();

}

// �I��
void PlayScene::Finalize()
{
	// �O���b�h���I��
	m_gridFloor.reset();
}

// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬����֐�
void PlayScene::CreateDeviceDependentResources()
{
	auto device = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();
	// �O���t�B�b�N�X�̐���
	m_graphics->GetInstance()->SetDeviceResources(GetUserResources()->GetDeviceResources());
	m_graphics->GetInstance()->SetCommonStates(states);
	m_graphics->Initialize();

	// ���\�[�X�����[�h
	Resources::GetInstance()->LoadResource();

	// �O���b�h�̏����쐬
	m_gridFloor = std::make_unique<Imase::GridFloor>(device, context, states);

	// �X�J�C�h�[�����쐬
	m_skydomeModel = Resources::GetInstance()->GetSkydome();

	// �X�e�[�W���쐬 
	GameResources gameResources = {&m_collisionManager};
	if (m_stage) m_stage->Kill();
	m_stage = m_taskManager.AddTask<Stage>(gameResources);
	
	// �X�e�[�W�f�[�^�̐ݒ�
	m_stage->SetStageData();

	// UI�̍쐬
	if (m_UI) m_UI->Kill();
	m_UI = m_taskManager.AddTask<UI>();
}

// �E�C���h�E�T�C�Y�Ɉˑ����郊�\�[�X���쐬����֐�
void PlayScene::CreateWindowSizeDependentResources()
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
void PlayScene::OnDeviceLost()
{
	Finalize();
}

