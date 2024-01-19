#include "pch.h"
#include "TitleScene.h"
#include "PlayScene.h"
#include "Utilities/Resources.h"

using namespace DirectX;

// �R���X�g���N�^
TitleScene::TitleScene()
	: m_view{}
	, m_proj{}
	, m_skydomeModel{}
	, m_tankBodyModel{}
	, m_tankTurretModel{}
	, m_bulletModel{}
	, m_rotate{}
	, m_bulletPos{}
	, m_angle(-45.0f)
	, m_bulletRecovery(30.0f)
	, m_count(0)
{
}

// ������
void TitleScene::Initialize()
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();

	// �J�����̈ʒu�ݒ�
	m_playerCamera.SetPlayer(SimpleMath::Vector3::Zero, m_rotate);
	// �J�����̐ݒ�
	m_playerCamera.SetType(PlayerCamera::Type::TYPE_B);
}

// �X�V
void TitleScene::Update(const DX::StepTimer& timer)
{
	// �L�[�{�[�h�擾
	auto keyState = GetUserResources()->GetKeyboardStateTracker();

	// �J�����X�V
	m_playerCamera.Update((float)timer.GetElapsedSeconds());

	// �V�[���؂�ւ�
	auto transitionMask = GetUserResources()->GetTransitionMask();
	if (keyState->pressed.Enter)
	{
		transitionMask->SetCreateMaskRequest(TransitionMask::CreateMaskRequest::COPY);
		ChangeScene<PlayScene>();
		transitionMask->SetCreateMaskRequest(TransitionMask::CreateMaskRequest::NONE);
	}

	// PushEnter�̓_��
	m_count++;
	if (m_count >= 120)	m_count -= 120;

	// �C�e�̑��x�̌v�Z
	SimpleMath::Vector3 velocity = SimpleMath::Vector3::TransformNormal(BULLET_SPEED, 
								   SimpleMath::Matrix::CreateRotationY(m_angle));
	// �C�e�̈ړ�
	m_bulletPos += velocity;

	// �C�e���
	if (m_bulletPos.x <= -m_bulletRecovery ||
		m_bulletPos.x >= m_bulletRecovery  ||
		m_bulletPos.y <= -m_bulletRecovery ||
		m_bulletPos.y >= m_bulletRecovery 
		)
	{
		m_bulletPos = SimpleMath::Vector3::Zero;
	}
}	

// �`��
void TitleScene::Render()
{
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states = GetUserResources()->GetCommonStates();

	// �r���[�s���ݒ�
	m_view = SimpleMath::Matrix::CreateLookAt(
		m_playerCamera.GetEyePosition(),
		m_playerCamera.GetTargetPosition(),
		SimpleMath::Vector3::UnitY);

	// �O���b�h�̏���`��
	m_gridFloor->Render(context, m_view, m_proj);

	// �X�J�C�h�[���̕`��
	m_skydomeModel->Draw(context, *states, SimpleMath::Matrix::Identity, m_view, m_proj);

	// ��Ԃ̕`��
	SimpleMath::Matrix world;
	m_rotate = SimpleMath::Quaternion::CreateFromAxisAngle(SimpleMath::Vector3::Up, m_angle);
	world *= SimpleMath::Matrix::CreateFromQuaternion(m_rotate);
	world *= SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3::Zero);
	m_tankBodyModel->Draw(context, *states, world, m_view, m_proj);
	m_tankTurretModel->Draw(context, *states, world, m_view, m_proj);

	// �C�e�̕`��
	SimpleMath::Matrix bulletWorld = SimpleMath::Matrix::Identity;
	bulletWorld *= SimpleMath::Matrix::CreateFromQuaternion(m_rotate);
	bulletWorld *= SimpleMath::Matrix::CreateTranslation(m_bulletPos);
	m_bulletModel->Draw(context, *states, bulletWorld, m_view, m_proj);

	// �e�N�X�`���̕`��
	m_graphics->GetSpriteBatch()->Begin();
	m_graphics->GetSpriteBatch()->Draw(m_titleSRV.Get(), SimpleMath::Vector2(173.0f, 25.0f));
	if (m_count <= 60)
	{
		m_graphics->GetSpriteBatch()->Draw(m_pushSRV.Get(), SimpleMath::Vector2(0.0, 550.0f));
	}
	m_graphics->GetSpriteBatch()->End();
}

// �I��
void TitleScene::Finalize()
{
	m_gridFloor.reset();
	m_titleSRV.Reset();
	m_pushSRV.Reset();
}

// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬����֐�
void TitleScene::CreateDeviceDependentResources()
{
	auto device  = GetUserResources()->GetDeviceResources()->GetD3DDevice();
	auto context = GetUserResources()->GetDeviceResources()->GetD3DDeviceContext();
	auto states  = GetUserResources()->GetCommonStates();

	// �O���t�B�b�N�X�̐���
	m_graphics->SetDeviceResources(GetUserResources()->GetDeviceResources());
	m_graphics->SetCommonStates(states);
	m_graphics->Initialize();

	// ���\�[�X�����[�h
	Resources::GetInstance()->LoadResource();

	// �O���b�h���𐶐�
	m_gridFloor = std::make_unique<Imase::GridFloor>(device, context, states);

	// �^�C�g���e�N�X�`���̓ǂݍ���
	DX::ThrowIfFailed(
		DirectX::CreateDDSTextureFromFile(device,
			L"Resources/dds/TankWar.dds",
			nullptr,
			m_titleSRV.ReleaseAndGetAddressOf())
	);

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
	// �C�e���f���̍쐬
	m_bulletModel     = Resources::GetInstance()->GetBullet();
}

// �E�C���h�E�T�C�Y�Ɉˑ����郊�\�[�X���쐬����֐�
void TitleScene::CreateWindowSizeDependentResources()
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
void TitleScene::OnDeviceLost()
{
	Finalize();
}
