//*********************************************************************
//			概要	：描画関係のクラス
// 
//			制作日	：6月7日
// 
//			制作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#ifndef GRAPHICS_DEFINED
#define GRAPHICS_DEFINED

#include "DeviceResources.h"

class Graphics final
{
public:
	// DirectX Graphicsクラスのインスタンスを取得
	static Graphics* const GetInstance();

	// スクリーンサイズを取得
	void GetScreenSize(int& width, int& height) const { width = m_screenW; height = m_screenH; }
	// スクリーンサイズを設定
	void SetScreenSize(const int& width, const int& height) { m_screenW = width; m_screenH = height; }
	// デバイスリソースを取得
	DX::DeviceResources* GetDeviceResources() const { return m_deviceResources; }
	// デバイスリソースを設定
	void SetDeviceResources(DX::DeviceResources* pDR) { m_deviceResources = (pDR); }
	// PrimitiveBatchクラスのインスタンスを取得
	DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* GetPrimitiveBatch() const { return m_primitiveBatch.get(); }
	// InputLayoutクラスのインスタンスを取得
	ID3D11InputLayout* GetInputLayout() const { return m_inputLayout.Get(); }
	// BasicEffectクラスのインスタンス取得
	DirectX::BasicEffect* GetBasicEffect() { return m_basicEffect.get(); }
	// CommonStatesクラスのインスタンスを取得
	DirectX::CommonStates* GetCommonStates() const { return m_commonStates; }
	// CommonStatesクラスのインスタンスを設定
	void SetCommonStates(DirectX::CommonStates* pCS) { m_commonStates = pCS; }
	// SpriteBatchクラスのインスタンスを取得
	DirectX::SpriteBatch* GetSpriteBatch() { return m_spriteBatch.get(); }
	// SpriteFontクラスのインスタンスを取得
	DirectX::SpriteFont* GetFont() { return m_spriteFont.get(); }
	// EffectFactoryクラスのインスタンスを取得
	DirectX::EffectFactory* GetFX() const { return m_effectFactory.get(); }
	// ビュー行列を設定
	void SetViewMatrix(const DirectX::SimpleMath::Matrix& view) { m_view = view; }
	// ビュー行列を取得
	const DirectX::SimpleMath::Matrix& GetViewMatrix() { return m_view; };
	// 射影行列を設定
	void SetProjectionMatrix(const DirectX::SimpleMath::Matrix& projection) { m_projection = projection; }
	// 射影行列を取得
	const DirectX::SimpleMath::Matrix& GetProjectionMatrix() { return m_projection; };

public:
	// デストラクタ
	~Graphics();
	// 初期化
	void Initialize();
	// 文字列を描画
	void DrawString(const float& x, const float& y, const wchar_t* str);
	// プリミティブ描画を開始
	void DrawPrimitiveBegin(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection);
	// プリミティブ描画を終了
	void DrawPrimitiveEnd();
	// モデルを描画
	void DrawModel(const DirectX::Model* model, const DirectX::SimpleMath::Matrix& world, const bool& depthBuffer = true);

private:
	// コンストラクタ
	Graphics();

	// 代入は許容しない
	void operator=(const Graphics& object) = delete;
	// コピーコンストラクタは許容しない
	Graphics(const Graphics& object) = delete;

private:
	// Graphicsクラスのインスタンスへのポインタ
	static std::unique_ptr<Graphics> m_graphics;
	// デバイスリソースへのポインタ
	 DX::DeviceResources* m_deviceResources;
	// コモンステート
	DirectX::CommonStates* m_commonStates;
	// スプライトバッチ
	std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
	// スプライトフォント
	std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
	// ベーシックエフェクト
	std::unique_ptr<DirectX::BasicEffect> m_basicEffect;
	// プリミティブバッチ
	std::unique_ptr<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>> m_primitiveBatch;
	// エフェクトファクトリー
	std::unique_ptr<DirectX::EffectFactory> m_effectFactory;
	// ラスタライザーステート
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterrizerState;
	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
	// スクリーンサイズ
	int m_screenW;
	int m_screenH;
	// ビュー行列
	DirectX::SimpleMath::Matrix m_view;
	// 射影行列
	DirectX::SimpleMath::Matrix m_projection;
	// デバイス
	ID3D11Device* m_device;
	// デバイスコンテキスト
	ID3D11DeviceContext* m_context;
};
#endif		// GRAPHICS_DEFINED