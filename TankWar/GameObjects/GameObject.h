//*********************************************************************
//			概要	：ゲームオブジェクトクラス
// 
//			制作日	：11月2日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "MyLib/TaskManager.h"
#include "GameParameter.h"

class GameObject : public Task
{
private:

	// オブジェクト識別ID
	int m_id;

	// 半径
	float m_radius;

	// 質量
	float m_weight;

	// 床との摩擦係数
	float m_friction;

	// 加速度
	DirectX::SimpleMath::Vector3 m_acceleration;

	// 速度
	DirectX::SimpleMath::Vector3 m_velocity;

	// 位置
	DirectX::SimpleMath::Vector3 m_position;

	// 回転
	DirectX::SimpleMath::Quaternion m_rotate;

public:

	// コンストラクタ
	GameObject(
		int id,
		DirectX::SimpleMath::Vector3 position,
		DirectX::SimpleMath::Quaternion rotate,
		float radius,
		float friction,
		float weight
	);

	// 更新
	bool Update(float elapsedTime) override;

	// オブジェクト識別IDを設定
	void SetID(int id) { m_id = id; }

	// オブジェクト識別IDを取得
	int GetID() { return m_id; }

	// 衝突したら呼ばれる関数
	virtual void OnHit(GameObject* object) = 0;

	// 半径を取得する関数
	float GetRadius() { return m_radius; }

	// 速度を設定する関数
	void SetVelocity(DirectX::SimpleMath::Vector3 velocity) { m_velocity = velocity; }

	// 速度を取得する関数
	const DirectX::SimpleMath::Vector3& GetVelocity() { return m_velocity; }

	// 位置を設定する関数
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }

	// 位置を取得する関数
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }

	// 回転を設定する関数
	void SetRotate(DirectX::SimpleMath::Quaternion rotate) { m_rotate = rotate; }

	// 回転を取得する関数
	const DirectX::SimpleMath::Quaternion& GetRotate() { return m_rotate; }

	// 力を加える関数
	void AddForce(DirectX::SimpleMath::Vector3 dir, float force);

	// 摩擦係数を設定する関数
	void SetFriction(float friction) { m_friction = friction; }

	// 重さを設定する関数
	void SetWeight(float weight) { m_weight = weight; }

	// 速さを制限する関数
	void LimitSpeed(float speed);

	// 衝突時の力を取得する
	float GetHitForce();

};
