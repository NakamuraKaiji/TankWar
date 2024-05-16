//*********************************************************************
//			概要	：タンククラス
// 
//			制作日	：10月4日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "MyLib/TaskManager.h"
#include "GameObject.h"
#include "GameParameter.h"
#include "MyLib/ModelPart.h"
#include "CollisionManager.h"
#include "GameResources.h"


class PlayerTank : public GameObject
{
public:

	// プレイヤーの状態
	enum class PlayerState
	{
		Normal,		// 通常
		Hit,        // 衝突
	};

private:

	// 衝突中関数
	void Hit();

	// 砲弾との衝突関数
	void OnHit_Bullet();

	// 敵との当たり判定
	void OnHit_Player();

public:

	// 車体回転を設定する関数
	void SetBodyRotate(DirectX::SimpleMath::Quaternion rotate) { m_bodyRotate = rotate; }

	// 車体回転を取得する関数
	DirectX::SimpleMath::Quaternion GetBodyRotate() { return m_bodyRotate; }

	// 砲身回転を設定する関数
	void SetTurretRotate(DirectX::SimpleMath::Quaternion rotate) { m_turretRotate = rotate; }

	// 砲身回転を取得する関数
	DirectX::SimpleMath::Quaternion GetTurretRotate() { return m_turretRotate; }

	// ターゲットを設定
	void SetTarget(GameObject* target) { m_target = target; }


	// プレイヤーの状態を取得する関数
	PlayerState GetState() { return m_playerState; }

	// 弾を発射したどうかを取得する関数
	bool GetShotFlag() { return m_shotFlag; }

	// スタートしたかどうかを設定する関数
	void SetStartFlag(bool start);

public:

	// コンストラクタ
	PlayerTank(const GameResources& gameResources, DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotate);

	// 初期化
	void Initialize();

	// 更新
	bool Update(float elapsedTime) override;

	// 描画
	void Render() override;

	// 衝突したら呼ばれる関数
	void OnHit(GameObject* object) override;

	// 移動関数
	void Move(DirectX::Keyboard::KeyboardStateTracker* tracker, float elapsedTime);

	//	リセット
	void Reset();

private:

	// グラフィックス
	Graphics* m_graphics = Graphics::GetInstance();

	// 戦車の各パーツへのポインタ
	std::shared_ptr<DirectX::Model> m_bodyModel;
	std::shared_ptr<DirectX::Model> m_turretModel;
	enum {ROOT , BODY, TURRET, PARTS_CNT};

	// 戦車各パーツへのポインタ
	std::unique_ptr<ModelPart> m_parts[PARTS_CNT];

	// プレイヤーの状態
	PlayerState m_playerState;

	// Collisionで作成したリソース
	GameResources m_gameResources;

	// ターゲット
	GameObject* m_target;

private:	
	// 車体の回転
	DirectX::SimpleMath::Quaternion m_bodyRotate;

	// 砲身の回転
	DirectX::SimpleMath::Quaternion m_turretRotate;

	// 煙の出る間隔
	float m_smokeTime;

	// 砲弾の発射間隔
	float m_bulletTime;

	// 発射したかどうか
	bool m_shotFlag;

	// スタートしたかどうか
	bool m_start;
};
