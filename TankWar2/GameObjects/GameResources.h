#pragma once

#include "ImaseLib/ResourceManager.h"
#include "CollisionManager.h"
#include "InfoWindow/InfoWindow.h"

// �Q�[���ɕK�v�ȃ��\�[�X
struct GameResources
{
	// ���\�[�X�}�l�[�W���[�ւ̃|�C���^
	Imase::ResourceManager* resourceManager;

	// �x�[�V�b�N�G�t�F�N�g�i�G�t�F�N�g�p�j
	DirectX::BasicEffect* pBasicEffect;

	// �v���~�e�B�u�o�b�`�i�G�t�F�N�g�p�j
	DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* pPrimitiveBatch;

	// ���̓��C�A�E�g�i�G�t�F�N�g�p�j
	ID3D11InputLayout* m_pInputLayout;

	// �f�o�C�X�R���e�L�X�g�ւ̃|�C���^
	ID3D11DeviceContext* pContext;

	// ���ʃX�e�[�g�ւ̃|�C���^
	DirectX::CommonStates* pStates;

	// �X�v���C�g�o�b�`�ւ̃|�C���^
	DirectX::SpriteBatch* pSpriteBatch;

	// �r���[�s��ւ̃|�C���^
	DirectX::SimpleMath::Matrix* pView;

	// �ˉe�s��ւ̃|�C���^
	DirectX::SimpleMath::Matrix* pProj;

	// �Փ˔���p�I�u�W�F�N�g
	CollisionManager* pCollisionManager;

	// ���E�C���h�E�ւ̃|�C���^
	InfoWindow* pInfoWindow;

	// ���_�����Z����֐�
	std::function<void(int score)> addScore;
};

