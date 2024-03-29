//*********************************************************************
//			概要	：画面切り替え用のマスク表示クラス
// 
//			制作日	：1月18日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "StepTimer.h"

class TransitionMask
{
public:

	enum class CreateMaskRequest
	{
		NONE,	// リクエストなし
		COPY,	// フレームバッファの内容をコピー
	};

private:

	// 割合(0〜1)
	float m_rate;

	// オープン又はクローズするまでの時間
	float m_interval;

	// オープンフラグ（trueの場合オープン）
	bool m_open;

	// グラフィックスへのポインタ
	Graphics* m_graphics = Graphics::GetInstance();

	// 作成リクエスト
	CreateMaskRequest m_request;

	// ピクセルシェーダー
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PS_Fade;

	// 定数バッファの構造体
	struct ConstantBuffer
	{
		float rate;		// 割合（0〜1）
		float pad[3];
	};

	// 定数バッファへのポインタ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;

	// マスク用テクスチャ
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_maskTexture;

public:

	// コンストラクタ
	TransitionMask(
		ID3D11Device* device,
		float interval = 1.0f
	);

	// 更新処理
	void Update(float elapsedTime);

	// 描画処理
	void Draw(
		ID3D11DeviceContext* context,
		DirectX::CommonStates* states,
		ID3D11ShaderResourceView* texture,
		const RECT& rect
	);

	// オープンする関数
	void Open();

	// クローズする関数
	void Close();

	// 状態を確認する関数
	bool IsOpen();
	bool IsClose();

	// 割合（0〜1）を取得する関数
	float GetRate() const
	{
		if (m_open) return 1.0f - m_rate;
		return m_rate;
	}

	// 完了しているか確認する関数
	bool IsEnd() const
	{
		if (GetRate() == 1.0f) return true;
		return false;
	}

	// オープン、クローズまでの時間を設定する関数
	void SetInterval(float interval) { m_interval = interval; }

	// マスク作成リクエストの取得
	CreateMaskRequest GetCreateMaskRequest() const { return m_request; }

	// マスク作成リクエストの設定
	void SetCreateMaskRequest(CreateMaskRequest request) { m_request = request; }

};
