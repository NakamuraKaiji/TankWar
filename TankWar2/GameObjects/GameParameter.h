#pragma once

#include <DirectXMath.h>

// オブジェクト前方向
static constexpr DirectX::SimpleMath::Vector3 OBJECT_FORWARD = DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f);

enum class ObjectID
{
	None,		// なし
	Player,		// プレイヤー
	Enemy,      // 敵
	Bullet,		// 砲弾
};


// 衝突した時に絶対に加わる最小の力
static constexpr float HIT_FORCE_MIN = 0.2f;


// ---- プレイヤー関連 ----- //

// プレイヤーが移動する時に加える力（1フレーム辺りに加える力）
static constexpr float PLAYER_MOVE_FORCE = 0.02f;

// プレイヤーの重さ
static constexpr float PLAYER_WEIGHT = 0.8f;

// プレイヤーの床に対する摩擦係数
static constexpr float PLAYER_FRICTION = 0.1f;

// プレイヤーの最大移動速度
static constexpr float PLAYER_MAX_SPEED = 0.06f;

// プレイヤーの車体の回転速度
static constexpr float PLAYER_BODY_ROTATE = 1.0f;

// プレイヤーの砲塔の回転速度
static constexpr float PLAYER_TURRET_ROTATE = 0.8f;

// プレイヤーの衝突判定用の球の半径
static constexpr float PLAYER_RADIUS = 0.6f;

// ---- 敵関連 ---- //

// 敵の重さ
static constexpr float ENEMT_WEIGHT = 0.8f;

// 敵の床に対する摩擦係数
static constexpr float ENEMY_FRICTION = 0.1f;

// 敵の最大移動速度
static constexpr float ENEMY_MAX_SPEED = 0.02f;

// 敵の思考時間（単位：秒）s
static constexpr float ENEMY_THINK_INTERVAL = 0.4f;

// 敵が移動する時に加える力
static constexpr float ENEMY_MOVE_FORCE = 0.02f;

// 敵の衝突判定用の球の半径
static constexpr float ENEMY_RADIUS = 0.6f;

// ---- 砲弾関連 ---- //

// 砲弾の重さ
static constexpr float BULLET_WEIGHT = 0.0f;

// 砲弾の床に対する摩擦係数
static constexpr float BULLET_FRICTION = 0.0f;

// 砲弾の移動速度
static constexpr DirectX::SimpleMath::Vector3 BULLET_SPEED = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.1f);

// 砲弾の発射間隔
static constexpr float BULLET_FIRE_INTERVAL = 2.0f;

// 砲弾の衝突判定用の球の半径
static constexpr float BULLET_RADIUS = 0.2f;

// 砲弾の回収座標
static constexpr float BULLET_COLLECT = 100.0f;
