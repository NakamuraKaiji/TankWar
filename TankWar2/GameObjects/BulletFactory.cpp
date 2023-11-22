#include "pch.h"
#include "BulletFactory.h"

// –C’e‚ğ¶¬
std::vector<std::unique_ptr<Bullet>> BulletFactory::CreateBullets(
	const int& number,
	const DirectX::SimpleMath::Vector3& position,
	const DirectX::SimpleMath::Quaternion& rotate,
	const float& scale,
	bool used
)
{
	// ”z—ñ‚ğéŒ¾
	std::vector<std::unique_ptr<Bullet>> bullets;
	// ”z—ñ‚ÌƒTƒCƒY‚ğİ’è
	bullets.resize(number);
	// ”z—ñ‚É–C’e‚ğŠi”[
	for (int i = 0; i < number; i++)
	{
		// –C’e‚ğ¶¬
		bullets[i] = std::make_unique<Bullet>(0, 0, position, rotate, scale, used);
		// –C’e‚ğ‰Šú‰»
		bullets[i]->Initialize();
	}
	// –C’e”z—ñ‚ğ•Ô‚·
	return bullets;
}