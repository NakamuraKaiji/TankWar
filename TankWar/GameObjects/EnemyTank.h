//*********************************************************************
//			概要	：敵タンクのクラス
// 
//			制作日	：10月17日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "GameObject.h"
#include "MyLib/ModelPart.h"
#include "CollisionManager.h"
#include "GameResources.h"
#include "EnemyHitPoint.h"

class EnemyTank : public GameObject
{
public:
	// 敵の状態
	enum class EnemyState
	{
		Normal,			// 通常
		Hit,			// 吹き飛ばされ状態
		Stop,           // 停止
	};

public:

	// ターゲットを設定
	void SetTarget(GameObject* target) { m_target = target; }

	// 状態を取得する関数
	EnemyState GetState() { return m_enemyState; }

	// 弾を発射したどうかを取得する関数
	bool GetShotFlag() { return m_shotFlag; }

	// スタートしたかどうかを設定する関数
	void SetStartFlag(bool start);

	// 敵の体力にアクセスする関数
	EnemyHitPoint* GetEnemyHitPoint() { return m_enemyHP; }

private:

	// 通常時の関数
	void Normal(float elapsedTime);

	// 衝突中関数
	void Hit();

	// 砲弾との衝突関数
	void OnHit_Bullet();

	// プレイヤーとの当たり判定
	void OnHit_Player();

public:

	// コンストラクタ
	EnemyTank(const GameResources& gameResources, DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotate);

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
	DirectX::SimpleMath::Vector3 Seek(const DirectX::SimpleMath::Vector3& targetPosition);

	// 逃走行動		
	DirectX::SimpleMath::Vector3 Flee(const DirectX::SimpleMath::Vector3& targetPosition);

	// 徘徊
	DirectX::SimpleMath::Vector3 Wander(float* wanderOriantation, float wanderRadius, float wanderDistance, float wanderRotate);


	// ベクトルの長さの切り捨て
	static DirectX::SimpleMath::Vector3 Truncate(const DirectX::SimpleMath::Vector3& v, float maxLength);

private:
	// グラフィックス
	Graphics* m_graphics = Graphics::GetInstance();

	// ターゲット
	GameObject* m_target;

	// 戦車の各パーツへのポインタ
	std::shared_ptr<DirectX::Model> m_bodyModel;
	std::shared_ptr<DirectX::Model> m_turretModel;
	enum { ROOT, BODY, TURRET, PARTS_CNT };
	// 戦車各パーツへのポインタ
	std::unique_ptr<ModelPart> m_parts[PARTS_CNT];

	// 敵の状態
	EnemyState m_enemyState;

	// Collisionで作成したリソース
	GameResources m_gameResources;

	// 敵のHP
	EnemyHitPoint* m_enemyHP;

private:
	// 車体の角度
	DirectX::SimpleMath::Quaternion m_bodyRotate;

	// 砲身の角度
	DirectX::SimpleMath::Quaternion m_turretRotate;

	// 速度[m/s]
	DirectX::SimpleMath::Vector3 m_velocity;

	// 質量[kg]
	float m_mass;    

	// 徘徊行動で使用する方向
	float m_wanderOrientation;   

	// 砲弾の発射間隔
	float m_bulletTime;

	// 無敵時間
	float m_invincibleTime;

	// 煙の出る間隔
	float m_smokeTime;

	// 発射したかどうか
	bool m_shotFlag;

	// スタートしたかどうか
	bool m_start;
};
