//*********************************************************************
//			�T�v	�F�����蔻��N���X
// 
//			�����	�F10��25��
// 
//			�����	�FKaiji Nakamura
//*********************************************************************
#pragma once
#ifndef COLLISION_DEFINED
#define COLLISION_DEFINED

class Collision
{
public:
	// �R���X�g���N�^
	Collision();
	// �f�X�g���N�^
	~Collision();
	// ������
	void Initialize();
	// �X�V
	void Update(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotate);
	// �Փ˔���
	bool Intersects();

private:
	// �Փ˔�����
	std::vector<DirectX::BoundingBox*> m_srcBoxes;

	// �Փ˔�����i���[���h��ԁj
	std::vector<DirectX::BoundingOrientedBox> m_dstOrientedBoxes;

};

#endif // !COLLISION_DEFINED
