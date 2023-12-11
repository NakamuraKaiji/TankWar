#pragma once

#include <DirectXMath.h>

// �I�u�W�F�N�g�O����
static constexpr DirectX::SimpleMath::Vector3 OBJECT_FORWARD = DirectX::SimpleMath::Vector3(0.0f, 0.0f, -1.0f);

enum class ObjectID
{
	None,		// �Ȃ�
	Player,		// �v���C���[
	Enemy,      // �G
	Bullet,		// �C�e
};


// �Փ˂������ɐ�΂ɉ����ŏ��̗�
static constexpr float HIT_FORCE_MIN = 0.2f;


// ---- �v���C���[�֘A ----- //

// �v���C���[���ړ����鎞�ɉ�����́i1�t���[���ӂ�ɉ�����́j
static constexpr float PLAYER_MOVE_FORCE = 0.02f;

// �v���C���[�̏d��
static constexpr float PLAYER_WEIGHT = 0.8f;

// �v���C���[�̏��ɑ΂��門�C�W��
static constexpr float PLAYER_FRICTION = 0.1f;

// �v���C���[�̍ő�ړ����x
static constexpr float PLAYER_MAX_SPEED = 0.06f;

// �v���C���[�̎ԑ̂̉�]���x
static constexpr float PLAYER_BODY_ROTATE = 1.0f;

// �v���C���[�̖C���̉�]���x
static constexpr float PLAYER_TURRET_ROTATE = 0.8f;

// �v���C���[�̏Փ˔���p�̋��̔��a
static constexpr float PLAYER_RADIUS = 0.6f;

// ---- �G�֘A ---- //

// �G�̏d��
static constexpr float ENEMT_WEIGHT = 0.8f;

// �G�̏��ɑ΂��門�C�W��
static constexpr float ENEMY_FRICTION = 0.1f;

// �G�̍ő�ړ����x
static constexpr float ENEMY_MAX_SPEED = 0.02f;

// �G�̎v�l���ԁi�P�ʁF�b�js
static constexpr float ENEMY_THINK_INTERVAL = 0.4f;

// �G���ړ����鎞�ɉ������
static constexpr float ENEMY_MOVE_FORCE = 0.02f;

// �G�̏Փ˔���p�̋��̔��a
static constexpr float ENEMY_RADIUS = 0.6f;

// ---- �C�e�֘A ---- //

// �C�e�̏d��
static constexpr float BULLET_WEIGHT = 0.0f;

// �C�e�̏��ɑ΂��門�C�W��
static constexpr float BULLET_FRICTION = 0.0f;

// �C�e�̈ړ����x
static constexpr DirectX::SimpleMath::Vector3 BULLET_SPEED = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.1f);

// �C�e�̔��ˊԊu
static constexpr float BULLET_FIRE_INTERVAL = 2.0f;

// �C�e�̏Փ˔���p�̋��̔��a
static constexpr float BULLET_RADIUS = 0.2f;

// �C�e�̉�����W
static constexpr float BULLET_COLLECT = 100.0f;
