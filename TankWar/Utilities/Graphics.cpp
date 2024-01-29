#include "pch.h"
#include "Graphics.h"

std::unique_ptr<Graphics> Graphics::m_graphics = nullptr;

// DirectX Graphicsクラスのインスタンスを取得
Graphics* const Graphics::GetInstance()
{
	if (m_graphics == nullptr)
	{
		// DirectX Graphicsクラスのインスタンスを生成
		m_graphics.reset(new Graphics());
	}
	// DirectX Graphicsクラスのインスタンスを返す
	return m_graphics.get();
}

// コンストラクタ
Graphics::Graphics()
	:
	m_deviceResources(nullptr),			                // デバイスリソース
	m_commonStates(nullptr),				            // コモンステート
	m_spriteBatch(nullptr),					            // スプライトバッチ
	m_spriteFont(nullptr),						        // スプライトフォント
	m_basicEffect(nullptr),					            // ベーシックエフェクト
	m_primitiveBatch(nullptr),				            // プリミティブバッチ
	m_rasterrizerState(nullptr),				        // ラスタライザーステート
	m_effectFactory(nullptr),				            // エフェクトファクトリ
	m_inputLayout(nullptr),					            // 入力レイアウト
	m_screenW(0),								        // スクリーン幅
	m_screenH(0),									    // スクリーン高
	m_view{},											// ビュー行列
	m_projection{},								        // 射影行列
	m_device(nullptr),							        // デバイス
	m_context(nullptr)							        // デバイスコンテキスト
{
	// DeviceResourcesクラスのインスタンスを生成
	//m_deviceResources = std::make_unique<DX::DeviceResources>();
}

// デストラクタ
Graphics::~Graphics()
{
}

// 初期化
void Graphics::Initialize()
{
	// スクリーンサイズを設定
	//SetScreenSize(width, height);
	// デバイスリソースを設定
	//m_deviceResources = deviceResources;
	// デバイスを取得
	m_device = m_deviceResources->GetD3DDevice();
	// デバイスコンテキストを取得
	m_context = m_deviceResources->GetD3DDeviceContext();

	// コモンステートを生成
	//m_commonStates = std::make_unique<DirectX::CommonStates>(m_device);
	// スプライトバッチを生成
	m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(m_context);
	// ベーシックエフェクトを生成
	m_basicEffect = std::make_unique<DirectX::BasicEffect>(m_device);
	// スプライトフォントを生成
	m_spriteFont = std::make_unique<DirectX::SpriteFont>(m_device, L"resources\\font\\SegoeUI_18.spritefont");
	// プリミティブバッチを生成
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>>(m_context);
	// 入力レイアウトを生成
	m_basicEffect->SetVertexColorEnabled(true);
	// テクスチャを有効
	m_basicEffect->SetTextureEnabled(true);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	//// 入力レイアウトを生成
	//m_device->CreateInputLayout(
	//	DirectX::VertexPositionColor::InputElements,
	//	DirectX::VertexPositionColor::InputElementCount,
	//	shaderByteCode, byteCodeLength,
	//	m_inputLayout.ReleaseAndGetAddressOf()
	//);
	// 入力レイアウトを生成
	m_device->CreateInputLayout(
		DirectX::VertexPositionColorTexture::InputElements,
		DirectX::VertexPositionColorTexture::InputElementCount,
		shaderByteCode, byteCodeLength,
		m_inputLayout.ReleaseAndGetAddressOf()
	);
	CD3D11_RASTERIZER_DESC rasterizerStateDesc(
		D3D11_FILL_SOLID, D3D11_CULL_NONE, FALSE,
		D3D11_DEFAULT_DEPTH_BIAS, D3D11_DEFAULT_DEPTH_BIAS_CLAMP,
		D3D11_DEFAULT_SLOPE_SCALED_DEPTH_BIAS, TRUE, FALSE, FALSE, TRUE
	);
	// ラスタライズステートを生成
	m_device->CreateRasterizerState(&rasterizerStateDesc, m_rasterrizerState.ReleaseAndGetAddressOf());
	// エフェクトファクトリを生成
	m_effectFactory = std::make_unique<DirectX::EffectFactory>(m_device);
	// リソースディレクトリを設定
	m_effectFactory->SetDirectory(L"Resources/cmo");
}

// 文字列を描画
void Graphics::DrawString(const float& x, const float& y, const wchar_t* str)
{
	// 文字列を描画
	m_spriteFont->DrawString(m_spriteBatch.get(), str, DirectX::SimpleMath::Vector2(x, y));
}

// 描画プリミティブを開始
void Graphics::DrawPrimitiveBegin(const DirectX::SimpleMath::Matrix& view, const DirectX::SimpleMath::Matrix& projection)
{
	m_context->OMSetBlendState(m_commonStates->NonPremultiplied(), nullptr, 0xFFFFFFFF);
	m_context->OMSetDepthStencilState(m_commonStates->DepthRead(), 0);
	m_context->RSSetState(m_commonStates->CullCounterClockwise());
	//m_context->RSSetState(m_rasterrizeState.Get());

	// ビュー行列を設定
	m_basicEffect->SetView(view);
	// プロジェクション行列を設定
	m_basicEffect->SetProjection(projection);
	// ワールド行列を設定
	//m_basicEffect->SetWorld(DirectX::SimpleMath::Matrix::Identity);
	// 頂点カラーを有効
	m_basicEffect->SetVertexColorEnabled(true);
	// テクスチャを有効
	m_basicEffect->SetTextureEnabled(true);
	// 入力レイアウトを設定
	m_basicEffect->Apply(m_context);
	// 入力レイアウトを設定
	m_context->IASetInputLayout(m_inputLayout.Get());
	// プリミティブバッチを開始
	m_primitiveBatch->Begin();
}

// 描画プリミティブを終了
void Graphics::DrawPrimitiveEnd()
{
	// プリミティブバッチを終了
	m_primitiveBatch->End();
}

// モデルを描画
void Graphics::DrawModel(const DirectX::Model* model, const DirectX::SimpleMath::Matrix& world, const bool& depthBuffer)
{
	if (depthBuffer)
	{
		// モデルを描画
		model->Draw(m_context, *m_commonStates, world, m_view, m_projection);
	}
	else
	{
		// モデルを描画
		model->Draw(
			m_context,
			*m_commonStates,
			world,
			m_view,
			m_projection,
			false,
			[&]() { 	m_context->OMSetDepthStencilState(m_commonStates->DepthNone(), 0); }
		);
	}
}
