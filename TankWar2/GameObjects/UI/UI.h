//*********************************************************************
//			概要	：ユーザーインターフェースクラス
// 
//			制作日	：12月20日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "ImaseLib/TaskManager.h"
#include "Life.h"

class UI : public Imase::Task
{
public:

	// コンストラクタ
	UI();

	// 初期化
	void Initialize() override;

public:

	// 残機数にアクセスする関数
	Life* GetLife() { return m_life; }

public:

	// 画面サイズ
	static const int SCREEN_W = 1280;
	static const int SCREEN_H = 720;

private:

	// 残機テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_lifeTexture;

	// グラフィックスへのポインタ
	Graphics* m_graphics = Graphics::GetInstance();

	// 残機
	Life* m_life;
};