#include "pch.h"
#include "BulletFactory.h"

// �C�e�𐶐�
std::vector<std::unique_ptr<Bullet>> BulletFactory::CreateBullets(
	const int& number,
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Quaternion& rotate,
	const float& scale,
	bool used
)
{
	// �z���錾
	std::vector<std::unique_ptr<Bullet>> bullets;
	// �z��̃T�C�Y��ݒ�
	bullets.resize(number);
	// �z��ɖC�e���i�[
	for (int i = 0; i < number; i++)
	{
		// �C�e�𐶐�
		bullets[i] = std::make_unique<Bullet>(0, 0, position, rotate, scale, used);
		// �C�e��������
		bullets[i]->Initialize();
	}
	// �C�e�z���Ԃ�
	return bullets;
}