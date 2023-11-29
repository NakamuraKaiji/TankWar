//*********************************************************************
//			概要	：敵タンクのクラス
// 
//			制作日	：10月17日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#ifndef ENEMY_TANK_DEFINED
#define ENEMY_TANK_DEFIEND
#include "GameObject.h"
#include "GameParameter.h"
#include "ImaseLib/ModelPart.h"
#include "CollisionManager.h"

class EnemyTank : public GameObject
{
public:

	// ターゲットを設定
	void SetTarget(GameObject* target) { m_target = target; }

private:

	// 衝突中関数
	void Hit();

	// 砲弾との衝突関数
	void OnHit_Bullet(GameObject* object);

public:

	// コンストラクタ
	EnemyTank(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotate);

	// 初期化
	void Initialize();

	// 更新
	bool Update(float elapsedTime) override;

	// 描画
	void Render() override;

	// 衝突したら呼ばれる関数
	void OnHit(GameObject* object) override;

	//	リセット
	void Reset();

	// 指定の方向を向く
	void TurnHeading(const DirectX::SimpleMath::Vector3& direction);

public:
	// 操舵行動
	// 探索
	DirectX::SimpleMath::Vector3 Seek(const DirectX::SimpleMath::Vector3& targetPos);

	// 徘徊
	DirectX::SimpleMath::Vector3 Wander(float* wanderOriantation, float wanderRadius, float wanderDistance, float wanderRotate,  float time);

	// ベクトルの長さの切り捨て
	static DirectX::SimpleMath::Vector3 Truncate(const DirectX::SimpleMath::Vector3& v, float maxLength);
private:
	// グラフィックス
	Graphics* m_graphics = Graphics::GetInstance();

	// 衝突判定用オブジェクト
	CollisionManager pCollisionManager;

	// ターゲット
	GameObject* m_target;

	// 戦車の各パーツへのポインタ
	std::shared_ptr<DirectX::Model> m_bodyModel;
	std::shared_ptr<DirectX::Model> m_turretModel;
	enum { ROOT, BODY, TURRET, PARTS_CNT };
	// 戦車各パーツへのポインタ

	std::unique_ptr<Imase::ModelPart> m_parts[PARTS_CNT];

private:
	// 車体の角度
	DirectX::SimpleMath::Quaternion m_bodyRotate;
	// 砲身の角度
	DirectX::SimpleMath::Quaternion m_turretRotate;

	// 速度[m/s]
	DirectX::SimpleMath::Vector3 m_velocity;
	// 質量[kg]
	float m_mass;    
	// 最大移動速度(速さ)[m/s]
	float m_maxSpeed;   
	// 最大力[m/s2]
	float m_maxForce;   
	// 徘徊行動で使用する方向
	float m_wanderOrientation;   

	// 砲弾の発射間隔
	float m_coolTime;

};

#endif // !ENEMY_TANK_DEFINED

