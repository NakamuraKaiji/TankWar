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
#include "ImaseLib/TaskManager.h"
#include "ImaseLib/GridFloor.h"
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

	// �^�X�N�}�l�[�W���[
	Imase::TaskManager m_taskManager;

	// �O���b�h�̏��ւ̃|�C���^
	std::unique_ptr<Imase::GridFloor> m_gridFloor;

	// �X�J�C�h�[�����f���ւ̃|�C���^
	std::shared_ptr<DirectX::Model> m_skydomeModel;

	// �ԑ̃��f���ւ̃|�C���^
	std::shared_ptr<DirectX::Model> m_tankBodyModel;

	// �C�g���f���ւ̃|�C���^
	std::shared_ptr<DirectX::Model> m_tankTurretModel;

	// �������̐��̐�ԃ��f���ւ̃|�C���^
	std::shared_ptr<DirectX::Model> m_loseTankModel;

private:

	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;

	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;

	// �J�E���g
	int m_count;

	// �X�J�C�h�[���̉�]
	float m_skydomeRotate;

	// �ԑ̂̈ʒu
	DirectX::SimpleMath::Vector3 m_tankPosition;

	// ���̏o��Ԋu
	float m_smokeTime;

	// ����������������
	bool m_vectoryFlag;

};