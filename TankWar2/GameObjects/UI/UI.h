//*********************************************************************
//			�T�v	�F���[�U�[�C���^�[�t�F�[�X�N���X
// 
//			�����	�F12��20��
// 
//			�����	�FKaiji Nakamura
//*********************************************************************
#pragma once
#include "ImaseLib/TaskManager.h"
#include "Life.h"

class UI : public Imase::Task
{
public:

	// �R���X�g���N�^
	UI();

	// ������
	void Initialize() override;

public:

	// �c�@���ɃA�N�Z�X����֐�
	Life* GetLife() { return m_life; }

public:

	// ��ʃT�C�Y
	static const int SCREEN_W = 1280;
	static const int SCREEN_H = 720;

private:

	// �c�@�e�N�X�`��
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_lifeTexture;

	// �O���t�B�b�N�X�ւ̃|�C���^
	Graphics* m_graphics = Graphics::GetInstance();

	// �c�@
	Life* m_life;
};