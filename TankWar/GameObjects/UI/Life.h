//*********************************************************************
//			概要	：残機数を表示するクラス
// 
//			制作日	：12月20日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "MyLib/TaskManager.h"

class Life : public Task
{
private:

	// グラフィックスへのポインタ
	Graphics* m_graphics = Graphics::GetInstance();

	// テクスチャハンドル
	ID3D11ShaderResourceView* m_pTexture;

	// 表示位置
	int m_x, m_y;

	// 残機数
	int m_life;

public:
	// コンストラクタ
	Life(int x, int y);

	// 描画
	void Render() override;

public:

	// 残機数の取得
	int GetLife() { return m_life; }

	// 残機数の設定
	void SetLife(int life) { m_life = life; }

	//	画像を設定する
	void SetTexture(ID3D11ShaderResourceView* texture);


};
