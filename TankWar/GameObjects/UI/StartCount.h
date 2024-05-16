//*********************************************************************
//			概要	：ゲームが始まるまでのカウントクラス
// 
//			制作日	：5月15日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "MyLib/TaskManager.h"

class StartCount : public Task
{
private:
	// グラフィックスへのポインタ
	Graphics* m_graphics = Graphics::GetInstance();

	// テクスチャハンドル
	ID3D11ShaderResourceView* m_texture1;
	ID3D11ShaderResourceView* m_texture2;
	ID3D11ShaderResourceView* m_texture3;

	// 表示位置
	int m_x, m_y;

	// 画面サイズ
	int m_windowWidth, m_windowHeight;

	// カウント
	float m_count;

	bool m_startFlag;

public:
	// コンストラクタ
	StartCount(int x, int y, int windowWidth, int windowHeight);

	// 更新
	bool Update(float elapsedTime) override;

	// 描画
	void Render() override;

public:

	// カウントを取得
	float GetCount() { return m_count; }
	// カウントを設定
	void SetCount(float count);

	//	画像を設定する
	void SetTexture1(ID3D11ShaderResourceView* number1);
	void SetTexture2(ID3D11ShaderResourceView* number2);
	void SetTexture3(ID3D11ShaderResourceView* number3);

	// スタートフラグ
	bool GetStartFlag() { return m_startFlag; }


};
