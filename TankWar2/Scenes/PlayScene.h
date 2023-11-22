//*********************************************************************
//			�T�v	�F�v���C�V�[���N���X
// 
//			�����	�F10��3��
// 
//			�����	�FKaiji Nakamura
//*********************************************************************
#pragma once
#ifndef PLAYER_SCENE_DEFINED
#define PLAYER_SCENE_DEFINED
#include "ImaseLib/SceneManager.h"
#include "UserResources.h"
#include "ImaseLib/TaskManager.h"
#include "ImaseLib/GridFloor.h"
#include "GameObjects/PlayerCamera.h"
#include "GameObjects/Stage.h"

class PlayScene :public Imase::Scene<UserResources>
{
public:

	// �R���X�g���N�^
	PlayScene();

	// �f�X�g���N�^
	~PlayScene();

	// ������
	void Initialize() override;

	// �X�V
	void Update(const DX::StepTimer& timer) override;

	// �`��
	void Render() override;

	// �I������
	void Finalize() override;

	// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬����֐�
	void CreateDeviceDependentResources() override;

	// �E�C���h�E�T�C�Y�Ɉˑ����郊�\�[�X���쐬����֐�
	void CreateWindowSizeDependentResources() override;

	// �f�o�C�X���X�g�������ɌĂяo�����֐�
	void OnDeviceLost() override;

private:

	// �O���b�h�̏��ւ̃|�C���^
	std::unique_ptr<Imase::GridFloor> m_gridFloor;

	// �O���t�B�b�N�X�ւ̃|�C���^
	Graphics* m_graphics = Graphics::GetInstance();

	// �v���C���[�J����
	PlayerCamera m_playerCamera;

	// �X�J�C�h�[�����f���ւ̃|�C���^
	std::shared_ptr<DirectX::Model> m_skydomeModel;

	// �^�X�N�}�l�[�W���[
	Imase::TaskManager m_taskManager;

	// �X�e�[�W
	Stage* m_stage;
private:

	// �r���[�s��
	DirectX::SimpleMath::Matrix m_view;

	// �ˉe�s��
	DirectX::SimpleMath::Matrix m_proj;

};

#endif // !PLAYER_SCENE_DEFINED