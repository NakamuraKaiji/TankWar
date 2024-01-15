//*********************************************************************
//			�T�v	�F���U���g�V�[���N���X
// 
//			�����	�F1��9��
// 
//			�����	�FKaiji Nakamura
//*********************************************************************
#pragma once
#include "ImaseLib/SceneManager.h"
#include "UserResources.h"
#include "GameObjects/PlayerCamera.h"
#include "GameObjects/GameParameter.h"

class ResultScene : public Imase::Scene<UserResources>
{
public:

	// �R���X�g���N�^
	ResultScene();

	// ������
	void Initialize() override;

	// �X�V
	void Update(const DX::StepTimer& timer) override;

	// �`��
	void Render() override;

	// �I��
	void Finalize() override;

	// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬����֐�
	void CreateDeviceDependentResources() override;

	// �E�C���h�E�T�C�Y�Ɉˑ����郊�\�[�X���쐬����֐�
	void CreateWindowSizeDependentResources() override;

	// �f�o�C�X���X�g�������ɌĂяo�����֐�
	void OnDeviceLost() override;

private:
	// �O���t�B�b�N�X
	Graphics* m_graphics = Graphics::GetInstance();

	// PushEnter�e�N�X�`���n���h��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pushSRV;

	// �J����
	PlayerCamera m_playerCamera;


private:

	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;

	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;

	// �J�E���g
	int m_count;

	// �p�x
	DirectX::SimpleMath::Quaternion m_rotate;


};