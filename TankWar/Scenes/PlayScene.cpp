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
#include "ResultScene.h"

using namespace DirectX;

// �R���X�g���N�^
PlayScene::PlayScene()
	: m_view{}
	, m_proj{}
	, m_skydomeModel{}
	, m_skydomeRotate(0.0f)
	, m_stage{}
	, m_userInterface{}
	, m_life(0)
	, m_ratio(0.0f)
{
}

// ������
void PlayScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// �c�@����ݒ�
	m_life = PLAYER_CNT;
	m_userInterface->GetLife()->SetLife(m_life);

	// �G��HP��ݒ�
	m_ratio = ENEMY_HP;
	m_userInterface->GetEnemyHP()->SetRatio(m_ratio);

	// �}�X�N���I�[�v������
	auto transitionMask = GetUserResources()->GetTransitionMask();
	transitionMask->SetCreateMaskRequest(TransitionMask::CreateMaskRequest::NONE);
	transitionMask->Open();
}

// �X�V
void PlayScene::Update(const DX::StepTimer& timer)
{
	auto kbTracker = GetUserResources()->GetKeyboardStateTracker();

	// �^�X�N�̍X�V
	m_taskManager.Update((float)timer.GetElapsedSeconds());

	// �S�Ă̓G�𓮂���
	m_stage->StopAllEnemy(false);

	// �v���C���[�̈ړ�����
	m_stage->GetPlayer()->Move(kbTracker, (float)timer.GetElapsedSeconds());

	// �G�̈ʒu����n��
	m_userInterface->GetEnemyHP()->SetPosition(m_stage->GetEnemy()->GetPosition());
	m_userInterface->GetEnemyHP()->CreateBillboard(m_playerCamera.GetTargetPosition(), m_playerCamera.GetEyePosition(), SimpleMath::Vector3::Up);

	// �J�����̐ݒ�
	m_playerCamera.SetType(PlayerCamera::Type::TYPE_A);
	m_playerCamera.SetPlayer(m_stage->GetPlayer()->GetPosition(), m_stage->GetPlayer()->GetBodyRotate() * m_stage->GetPlayer()->GetTurretRotate());

	// �J�����̍X�V
	m_playerCamera.Update((float)timer.GetElapsedSeconds());
	
	// �Փ˔���̍X�V
	m_collisionManager.Update();

	// ����������c�@�����炷
	if (m_stage->GetPlayer()->GetState() == PlayerTank::PlayerState::Hit)
	{
		m_life--;
		m_userInterface->GetLife()->SetLife(m_life);
		if (m_life == 0)
		{
			// ���U���g�V�[���ɐ؂�ւ�
			ChangeScene<ResultScene>();
			GetUserResources()->SetVictoryFlag(false);
		}
	}
	// �f�o�b�O�p
	if (kbTracker->pressed.Z)
	{
		m_life--;
		m_userInterface->GetLife()->SetLife(m_life);
		if (m_life == 0)
		{
			// ���U���g�V�[���ɐ؂�ւ�
			ChangeScene<ResultScene>();
			GetUserResources()->SetVictoryFlag(false);
		}
	}

	// �G���C�e�ɓ���������HP�����炷
	if (m_stage->GetEnemy()->GetState() == EnemyTank::EnemyState::Hit)
	{
		m_ratio = m_ratio - ENEMY_REDUCE_HP;
		m_userInterface->GetEnemyHP()->SetRatio(m_ratio);
		if (m_ratio < -0.3f)
		{
			// ���U���g�V�[���ɐ؂�ւ�
			ChangeScene<ResultScene>();
			GetUserResources()->SetVictoryFlag(true);
		}
	}
	// �f�o�b�O�p
	if (kbTracker->pressed.X)
	{
		m_ratio = m_ratio - ENEMY_REDUCE_HP;
		m_userInterface->GetEnemyHP()->SetRatio(m_ratio);
		if (m_ratio < -0.3f)
		{
			// ���U���g�V�[���ɐ؂�ւ�
			ChangeScene<ResultScene>();
			GetUserResources()->SetVictoryFlag(true);
		}
	}
	else if (kbTracker->pressed.C)
	{
		// ���U���g�V�[���ɐ؂�ւ�
		ChangeScene<ResultScene>();
		GetUserResources()->SetVictoryFlag(true);
	}

	// �X�J�C�h�[���̉�]
	m_skydomeRotate += (float)timer.GetElapsedSeconds() * 0.05f;

}

// �`��
void PlayScene::Render()
{
	//auto debugFont = GetUserResources()->GetDebugFont();
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

	// �C���v�b�g���C�A�E�g��o�^
	m_graphics->GetDeviceResources()->GetD3DDeviceContext()->IASetInputLayout(m_graphics->GetInputLayout());

	// �n�ʂ�`��
	SimpleMath::Matrix groundWorld;
	groundWorld *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(0.0f, -0.5f, 0.0f));
	m_groundModel->Draw(context, *states, groundWorld, m_view, m_proj);

	// �X�J�C�h�[���̕`��
	SimpleMath::Matrix skyWorld;
	skyWorld *= SimpleMath::Matrix::CreateRotationY(m_skydomeRotate);
	skyWorld *= SimpleMath::Matrix::CreateTranslation(m_stage->GetPlayer()->GetPosition());
	m_skydomeModel->Draw(context, *states, skyWorld, m_view, m_proj);

	if (m_graphics->GetSpriteBatch()) m_graphics->GetSpriteBatch()->Begin();

	// �^�X�N�̕`��
	m_taskManager.Render();

	if (m_graphics->GetSpriteBatch()) m_graphics->GetSpriteBatch()->End();

}

// �I��
void PlayScene::Finalize()
{
}

// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬����֐�
void PlayScene::CreateDeviceDependentResources()
{
	auto states = GetUserResources()->GetCommonStates();

	// �O���t�B�b�N�X�̐���
	m_graphics->GetInstance()->SetDeviceResources(GetUserResources()->GetDeviceResources());
	m_graphics->GetInstance()->SetCommonStates(states);
	m_graphics->Initialize();

	// ���\�[�X�����[�h
	Resources::GetInstance()->LoadResource();

	// �n�ʃ��f���̍쐬
	m_groundModel = Resources::GetInstance()->GetGround();
	// �X�J�C�h�[�����쐬
	m_skydomeModel = Resources::GetInstance()->GetSkydome();

	// �X�e�[�W���쐬 
	GameResources gameResources = {&m_collisionManager};
	if (m_stage) m_stage->Kill();
	m_stage = m_taskManager.AddTask<Stage>(gameResources);
	
	// �X�e�[�W�f�[�^�̐ݒ�
	m_stage->SetStageData();

	// UI�̍쐬
	if (m_userInterface) m_userInterface->Kill();
	m_userInterface = m_taskManager.AddTask<UserInterface>();
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

