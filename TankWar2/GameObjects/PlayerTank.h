//*********************************************************************
//			概要	：タンククラス
// 
//			制作日	：10月4日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#ifndef TANK_DEFINED
#define TANK_DEFINED
#include "ImaseLib/TaskManager.h"
#include "GameObject.h"
#include "GameParameter.h"
#include "ImaseLib/ModelPart.h"
#include "CollisionManager.h"

class PlayerTank : public GameObject
{
private:

	// 衝突中関数
	void Hit();

	// 砲弾との衝突関数
	void OnHit_Bullet(GameObject* object);

public:

	// コンストラクタ
	PlayerTank(int x, int y);

	// 初期化
	void Initialize();

	// 更新
	bool Update(float elapsedTime) override;

	// 描画
	void Render() override;

	// 衝突したら呼ばれる関数
	void OnHit(GameObject* object) override;

	// 移動関数
	void Move(DirectX::Keyboard::KeyboardStateTracker* tracker);

	//	終了
	void Finalize();
private:

	// グラフィックス
	Graphics* m_graphics = Graphics::GetInstance();

	// 衝突判定用オブジェクト
	CollisionManager pCollisionManager;

	// 戦車の各パーツへのポインタ
	std::shared_ptr<DirectX::Model> m_bodyModel;
	std::shared_ptr<DirectX::Model> m_turretModel;
	enum {ROOT , BODY, TURRET, PARTS_CNT};

	// 戦車各パーツへのポインタ
	std::unique_ptr<Imase::ModelPart> m_parts[PARTS_CNT];

private:	

	// 位置
	DirectX::SimpleMath::Vector3 m_position;

	// 車体の回転
	DirectX::SimpleMath::Quaternion m_bodyRotate;

	// 砲身の回転
	DirectX::SimpleMath::Quaternion m_turretRotate;
};

#endif // !TANK_DEFINED
