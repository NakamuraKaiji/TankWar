//*********************************************************************
//			概要	：ゲームオブジェクトクラス
// 
//			制作日	：11月2日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "ImaseLib/TaskManager.h"
#include "GameParameter.h"

class GameObject : public Imase::Task
{
private:
	// オブジェクト識別ID
	int m_id;

	// 初期配置位置
	int m_x, m_y;

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

	// 車体の回転
	DirectX::SimpleMath::Quaternion m_bodyRotate;

	// 砲身の回転
	DirectX::SimpleMath::Quaternion m_turretRotate;

	// 砲弾の位置
	DirectX::SimpleMath::Vector3 m_bulletPosition;

	// 砲弾の回転
	DirectX::SimpleMath::Quaternion m_bulletRotate;

public:

	// コンストラクタ
	GameObject(int id, int x, int y, float radius, float friction, float weight);

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

	// 戦車の位置を設定する関数
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }

	// 戦車の位置を取得する関数
	const DirectX::SimpleMath::Vector3& GetPosition() { return m_position; }

	// 車体回転を設定する関数
	void SetBodyRotate(DirectX::SimpleMath::Quaternion rotate) { m_bodyRotate = rotate; }

	// 車体回転を取得する関数
	DirectX::SimpleMath::Quaternion GetBodyRotate() { return m_bodyRotate; }

	// 砲身回転を設定する関数
	void SetTurretRotate(DirectX::SimpleMath::Quaternion rotate) { m_turretRotate = rotate; }

	// 砲身回転を取得する関数
	DirectX::SimpleMath::Quaternion GetTurretRotate() { return m_turretRotate; }

	// 力を加える関数
	void AddForce(DirectX::SimpleMath::Vector3 dir, float force);

	// 摩擦係数を設定する関数
	void SetFriction(float friction) { m_friction = friction; }

	// 重さを設定する関数
	void SetWeight(float weight) { m_weight = weight; }

	// 速さを制限する関数
	void LimitSpeed(float speed);

	// ステージ上での配置位置を取得する関数
	int GetX() { return m_x; }
	int GetY() { return m_y; }

	// 位置を設定する関数
	void SetBulletPosition(DirectX::SimpleMath::Vector3 position) { m_bulletPosition = position; }

	// 位置を取得する関数
	const DirectX::SimpleMath::Vector3& GetBulletPosition() { return m_bulletPosition; }

	// 砲弾回転を取得する関数
	DirectX::SimpleMath::Quaternion GetBulletRotate() { return m_bulletRotate; }

	// 砲弾回転を設定する関数
	void SetBulletRotate(DirectX::SimpleMath::Quaternion rotate) { m_bulletRotate = rotate; }

	// 砲弾発射関数
	void Shot(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion bodyRotate, DirectX::SimpleMath::Quaternion turretRotate);
};
