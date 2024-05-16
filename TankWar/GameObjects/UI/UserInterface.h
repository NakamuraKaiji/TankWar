//*********************************************************************
//			概要	：ユーザーインターフェースクラス
// 
//			制作日	：12月20日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "MyLib/TaskManager.h"
#include "Life.h"
#include "StartCount.h"

class UserInterface : public Task
{
public:

	// コンストラクタ
	UserInterface();

	// 初期化
	void Initialize() override;

public:

	// 残機数にアクセスする関数
	Life* GetLife() { return m_life; }

	// スタートカウントにアクセスする関数
	StartCount* GetStartCount(){return m_startCount;}

public:

	// 画面サイズ
	static const int SCREEN_W = 1280;
	static const int SCREEN_H = 720;

private:

	// 残機テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_lifeTexture;

	// 数字テクスチャ（１）
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture1;
	// 数字テクスチャ（２）
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture2;
	// 数字テクスチャ（３）
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture3;

	// グラフィックスへのポインタ
	Graphics* m_graphics = Graphics::GetInstance();

	// 残機
	Life* m_life;

	// スタートカウント
	StartCount* m_startCount;
};