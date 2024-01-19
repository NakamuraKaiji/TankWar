//*********************************************************************
//			概要	：プレイヤーのカメラクラス
// 
//			制作日	：7月03日
// 
//			制作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "Camera.h"

// プレイヤーカメラ
class PlayerCamera : public Camera
{
public:
	// カメラタイプ
	enum class Type
	{
		TYPE_A,
		TYPE_B,
		TYPE_C,
	};
private:
	// 現在のカメラタイプ
	Type m_type;
	// 回転角
	float m_angle;
	// プレイヤーの位置
	DirectX::SimpleMath::Vector3 m_playerPos;
	// プレイヤーの回転
	DirectX::SimpleMath::Quaternion m_playerRotate;
public:
	// コンストラクタ
	PlayerCamera();
	// 更新
	void Update(float elapsedTime);
	// プレイヤーの位置と回転角を設定	
	void SetPlayer(
		DirectX::SimpleMath::Vector3 position,
		DirectX::SimpleMath::Quaternion rotate
	)
	{
		m_playerPos = position;
		m_playerRotate = rotate;
	}
	// カメラのタイプを設定する関数
	void SetType(Type type)
	{
		// カメラの初期化を呼ぶ
		//InitializeCamera();
		m_type = type;
	}
};