//*********************************************************************
//			�T�v	�F�C�e�t�@�N�g���[�N���X�̃C���^�[�t�F�[�X
// 
//			�����	�F10��18��
// 
//			�����	�FKaiji Nakamura
//*********************************************************************
#pragma once
#ifndef IBULLET_FACTORY_DEFINED
#define IBULLET_FACTORY_DEFINED
#include <vector>
#include "GameObjects/Bullet.h"

class IBulletFactory
{
	// �w�肳�ꂽ�C�e�z��𐶐�
	virtual std::vector<std::unique_ptr<Bullet>> CreateBullets(
		const int& number,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Quaternion& rotate,
		const float& scale,
		bool used
	) = 0;
};

#endif // !IBULLET_FACTORY_DEFINED