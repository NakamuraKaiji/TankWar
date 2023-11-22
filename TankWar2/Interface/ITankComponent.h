//*********************************************************************
//			概要	：タンククラスのインターフェースクラス
// 
//			制作日	：10月16日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#ifndef ITANK_COMPONENT_DEFINED
#define ITANK_COMPONENT_DEFINED
#include "StepTimer.h"

class ITankComponent
{
public:
	// 位置を取得
	virtual DirectX::SimpleMath::Vector3 GetPosition() = 0;
	// 車体の回転角を取得
	virtual DirectX::SimpleMath::Quaternion GetBodyRotate() = 0;
	// 砲身の転角を取得
	virtual DirectX::SimpleMath::Quaternion GetTurretRotate() = 0;

	// 車体モデルを取得
	virtual DirectX::Model* GetBodyModel() = 0;
	// 砲身モデルを取得
	virtual DirectX::Model* GetTurretModel() = 0;

public:
	// デストラクタ
	virtual ~ITankComponent() = default;
	// 初期化
	virtual void Initialize() = 0;
	// 更新
	virtual void Update(const DX::StepTimer& timer) = 0;
	// 描画
	virtual void Render() = 0;
	// 終了
	virtual void Finalize() = 0;
};

#endif // !ITANK_COMPONENT_DEFINED


