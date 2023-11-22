//*********************************************************************
//			�T�v	�F�C�e�t�@�N�g���[�N���X
// 
//			�����	�F10��18��
// 
//			�����	�FKaiji Nakamura
//*********************************************************************
#pragma once
#ifndef BULLET_FACTORY_DEFIEND
#define BULLET_FACTORY_DEFIEND
#include <vector>
#include "Interface/IBulletFactory.h"
#include "Bullet.h"

class BulletFactory : public IBulletFactory
{
public:
	// �w�肳�ꂽ�C�e�z��̐���
	static std::vector<std::unique_ptr<Bullet>> CreateBullets(
		const int& number,
		const DirectX::SimpleMath::Vector3& position,
		const DirectX::SimpleMath::Quaternion& rotate,
		const float& scale,
		bool used
	);
};


#endif // !BULLET_FACTORY_DEFIEND