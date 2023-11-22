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
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionColor>>(m_context);
	// 入力レイアウトを生成
	m_basicEffect->SetVertexColorEnabled(true);
	// テクスチャを無効
	m_basicEffect->SetTextureEnabled(false);

	void const* shaderByteCode;
	size_t byteCodeLength;

	m_basicEffect->GetVertexShaderBytecode(&shaderByteCode, &byteCodeLength);
	// 入力レイアウトを生成
	m_device->CreateInputLayout(
		DirectX::VertexPositionColor::InputElements,
		DirectX::VertexPositionColor::InputElementCount,
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
	m_effectFactory->SetDirectory(L"resources/cmo");
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
	m_context->OMSetBlendState(m_commonStates->Opaque(), nullptr, 0xFFFFFFFF);
	m_context->OMSetDepthStencilState(m_commonStates->DepthNone(), 0);
	m_context->RSSetState(m_commonStates->CullNone());
	//m_context->RSSetState(m_rasterrizeState.Get());

	// ビュー行列を設定
	m_basicEffect->SetView(view);
	// プロジェクション行列を設定
	m_basicEffect->SetProjection(projection);
	// ワールド行列を設定
	m_basicEffect->SetWorld(DirectX::SimpleMath::Matrix::Identity);

	// 頂点カラーを有効
	m_basicEffect->SetVertexColorEnabled(true);
	// テクスチャを有効
	m_basicEffect->SetTextureEnabled(false);
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

// 線分を描画(XZ平面)
void Graphics::DrawLine(const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& vector, const DirectX::FXMVECTOR& color)
{
	// 頂点カラーを設定
	DirectX::VertexPositionColor vertex[2] =
	{
		{ DirectX::SimpleMath::Vector3(position.x, 0.0f, position.y), color },
		{ DirectX::SimpleMath::Vector3(position.x + vector.x, 0.0f, position.y + vector.y), color }
	};
	// 線分を描画
	m_primitiveBatch->DrawLine(vertex[0], vertex[1]);
}

// 線分を描画
void Graphics::DrawLine(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& vector, const DirectX::FXMVECTOR& color)
{
	// 頂点カラーを設定
	DirectX::VertexPositionColor vertex[2] =
	{
		{ DirectX::SimpleMath::Vector3(position.x, position.y, position.z), color },
		{ DirectX::SimpleMath::Vector3(position.x + vector.x, position.y + vector.y, position.z + vector.z), color }
	};
	// 線分を描画
	m_primitiveBatch->DrawLine(vertex[0], vertex[1]);
}

// ベクトルを描画(XZ平面)
void Graphics::DrawVector(const DirectX::SimpleMath::Vector2& position, const DirectX::SimpleMath::Vector2& vector, const DirectX::FXMVECTOR& color)
{
	using namespace DirectX::SimpleMath;
	const float cosTheta = cosf(DirectX::XMConvertToRadians(20.0f));
	const float sinTheta = sinf(DirectX::XMConvertToRadians(20.0f));

	// 矢印のベクトルのサイズを設定
	Vector2 arrow = -vector;
	// 正規化
	arrow.Normalize();
	// 矢印のサイズを設定
	arrow *= 3.0f;
	// 右矢 X: (xcosθ- ysinθ)  Y: (xsinθ+ ycosθ)
	Vector2 arrowR = Vector2(arrow.x * cosTheta - arrow.y * sinTheta, arrow.x * sinTheta + arrow.y * cosTheta);
	// 左矢 X: (xcosθ- ysinθ)  Y: (xsinθ+ ycosθ)
	Vector2 arrowL = Vector2(arrow.x * cosTheta + arrow.y * sinTheta, -arrow.x * sinTheta + arrow.y * cosTheta);
	// 線分を描画
	DrawLine(position, vector, color);
	// 右矢を描画
	DrawLine(position + vector, arrowR, color);
	// 左矢を描画
	DrawLine(position + vector, arrowL, color);
}

// ベクトルを描画
void Graphics::DrawVector(const DirectX::SimpleMath::Vector3& position, const DirectX::SimpleMath::Vector3& vector, const DirectX::FXMVECTOR& color)
{
	using namespace DirectX::SimpleMath;
	const float cosTheta = cosf(DirectX::XMConvertToRadians(20.0f));
	const float sinTheta = sinf(DirectX::XMConvertToRadians(20.0f));

	// 矢印のベクトルのサイズを設定
	Vector3 arrow = -vector;
	// 正規化
	arrow.Normalize();
	// 矢印のサイズを設定
	arrow *= 3.0f;
	// 右矢 X: (xcosθ- zsinθ)  Z: (xsinθ+ zcosθ)
	Vector3 arrowR = Vector3(arrow.x * cosTheta - arrow.z * sinTheta, arrow.y, arrow.x * sinTheta + arrow.z * cosTheta);
	// 左矢 X: (xcosθ- zsinθ)  Z: (xsinθ+ zcosθ)
	Vector3 arrowL = Vector3(arrow.x * cosTheta + arrow.z * sinTheta, arrow.y, -arrow.x * sinTheta + arrow.z * cosTheta);
	// 線分を描画
	DrawLine(position, vector, color);
	// 右矢を描画
	DrawLine(position + vector, arrowR, color);
	// 左矢を描画
	DrawLine(position + vector, arrowL, color);
}

// 円を描画(XZ平面)
void Graphics::DrawCircle(const DirectX::SimpleMath::Vector2& center, const float& radius, const DirectX::FXMVECTOR& color, const int& split)
{
	using namespace DirectX::SimpleMath;

	// 角度を初期化
	float angle = 0.0f;
	// 始点を宣言
	Vector2 position1 = center + Vector2(cosf(angle), sinf(angle)) * radius;
	for (int index = 0; index < split; index++)
	{
		// 始点を設定
		Vector2 position0 = position1;
		// 角度を計算
		angle += DirectX::XM_2PI / (float)split;
		// 終点を計算
		position1 = center + Vector2(cosf(angle), sinf(angle)) * radius;
		// 円を描画
		DrawLine(position0, position1 - position0, color);
	}
}

// 円を描画
void Graphics::DrawCircle(const DirectX::SimpleMath::Vector3& center, const float& radius, const DirectX::FXMVECTOR& color, const int& split)
{
	using namespace DirectX::SimpleMath;

	// 角度を初期化
	float angle = 0.0f;
	// 終点を計算
	Vector3 position1 = center + Vector3(cosf(angle), center.y, sinf(angle)) * radius;
	for (int index = 0; index < split; index++)
	{
		// 始点を設定
		Vector3 position0 = position1;
		// 角度を計算
		angle += DirectX::XM_2PI / (float)split;
		// 終点を計算
		position1 = center + Vector3(cosf(angle), center.y, sinf(angle)) * radius;
		// 円を描画
		DrawLine(position0, position1 - position0, color);
	}
}

// 矩形を描画
void Graphics::DrawQuad(const DirectX::VertexPositionColor& v1, const DirectX::VertexPositionColor& v2, const DirectX::VertexPositionColor& v3, const DirectX::VertexPositionColor& v4)
{
	// 矩形を描画
	m_primitiveBatch->DrawQuad(v1, v2, v3, v4);
}

// 矩形を描画
void Graphics::DrawQuad(
	const DirectX::FXMVECTOR& point1,
	const DirectX::FXMVECTOR& point2,
	const DirectX::FXMVECTOR& point3,
	const DirectX::FXMVECTOR& point4,
	const DirectX::HXMVECTOR& color
)
{
	DirectX::VertexPositionColor vertexs[5];
	DirectX::XMStoreFloat3(&vertexs[0].position, point1);
	DirectX::XMStoreFloat3(&vertexs[1].position, point2);
	DirectX::XMStoreFloat3(&vertexs[2].position, point3);
	DirectX::XMStoreFloat3(&vertexs[3].position, point4);
	DirectX::XMStoreFloat3(&vertexs[4].position, point1);

	XMStoreFloat4(&vertexs[0].color, color);
	XMStoreFloat4(&vertexs[1].color, color);
	XMStoreFloat4(&vertexs[2].color, color);
	XMStoreFloat4(&vertexs[3].color, color);
	XMStoreFloat4(&vertexs[4].color, color);

	m_primitiveBatch->Draw(D3D_PRIMITIVE_TOPOLOGY_LINESTRIP, vertexs, 5);
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

// アニメーションモデルを描画
//void Graphics::DrawModel(
//	const DirectX::Model* model,
//	const DX::AnimationSDKMESH* animationSDKMESH,
//	const DirectX::ModelBone::TransformArray* transformArray,
//	const DirectX::SimpleMath::Matrix& world
//)
//{
//	// ボーン配列のサイズを取得
//	size_t bones = model->bones.size();
//	// アニメーションにモデル、ボーン数、ボーンを適用
//	animationSDKMESH->Apply(*model, bones, transformArray->get());
//	// コモンステートを取得
//	DirectX::CommonStates* commonState = Graphics::GetInstance()->GetCommonStates();
//	// スキンメッシュアニメーションを描画
//	model->DrawSkinned(m_context, *commonState, bones, transformArray->get(), world, m_view, m_projection);
//}
