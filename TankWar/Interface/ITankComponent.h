//*********************************************************************
//			�T�v	�F�^���N�N���X�̃C���^�[�t�F�[�X�N���X
// 
//			�����	�F10��16��
// 
//			�����	�FKaiji Nakamura
//*********************************************************************
#pragma once
#ifndef ITANK_COMPONENT_DEFINED
#define ITANK_COMPONENT_DEFINED
#include "StepTimer.h"

class ITankComponent
{
public:
	// �ʒu���擾
	virtual DirectX::SimpleMath::Vector3 GetPosition() = 0;
	// �ԑ̂̉�]�p���擾
	virtual DirectX::SimpleMath::Quaternion GetBodyRotate() = 0;
	// �C�g�̓]�p���擾
	virtual DirectX::SimpleMath::Quaternion GetTurretRotate() = 0;

	// �ԑ̃��f�����擾
	virtual DirectX::Model* GetBodyModel() = 0;
	// �C�g���f�����擾
	virtual DirectX::Model* GetTurretModel() = 0;

public:
	// �f�X�g���N�^
	virtual ~ITankComponent() = default;
	// ������
	virtual void Initialize() = 0;
	// �X�V
	virtual void Update(const DX::StepTimer& timer) = 0;
	// �`��
	virtual void Render() = 0;
	// �I��
	virtual void Finalize() = 0;
};

#endif // !ITANK_COMPONENT_DEFINED


