#include "pch.h"
#include "EnemyHitPoint.h"
#include "Utilities/Resources.h"

using namespace DirectX;

//コンストラクタ
EnemyHitPoint::EnemyHitPoint()
	: m_hp(100.0f)
{
}

// 初期化
void EnemyHitPoint::Initialize()
{
	auto device = m_graphics->GetDeviceResources()->GetD3DDevice();
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	
	// ベーシックエフェクトの作成
	m_basicEffect = std::make_unique<BasicEffect>(device);
	m_basicEffect->SetLightingEnabled(false);
	m_basicEffect->SetVertexColorEnabled(false);
	m_basicEffect->SetTextureEnabled(true);

	// 入力レイアウトの作成
	DX::ThrowIfFailed(
		CreateInputLayoutFromEffect<VertexPositionTexture>(
			device,
			m_basicEffect.get(),
			m_inputLayout.ReleaseAndGetAddressOf())
	);

	// プリミティブバッチの作成
	m_primitiveBatch = std::make_unique<DirectX::PrimitiveBatch<DirectX::VertexPositionTexture>>(context);

	// テクスチャを取得する
	m_hpTexture_a = Resources::GetInstance()->GetHP_A();
	m_hpTexture_b = Resources::GetInstance()->GetHP_B();
}

// 描画
void EnemyHitPoint::Render()
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_graphics->GetCommonStates();

	// ビュー行列を取得
	SimpleMath::Matrix view = m_graphics->GetViewMatrix();
	// 射影行列を取得
	SimpleMath::Matrix proj = m_graphics->GetProjectionMatrix();

	// 体力バーの位置（戦車の位置からY軸に1上げた場所に位置している）
	SimpleMath::Vector3 objectPosition = m_position + SimpleMath::Vector3(0.0f, 1.0f, 0.0f);

	// ビルボード行列の作成
	SimpleMath::Matrix billboardMatrix = SimpleMath::Matrix::CreateBillboard(
		objectPosition,
		m_cameraPosition,
		SimpleMath::Vector3::Up,
		nullptr
	);

	// エフェクトの設定
	m_basicEffect->SetWorld(billboardMatrix);
	m_basicEffect->SetView(view);
	m_basicEffect->SetProjection(proj);

	// 半透明の設定（通常の半透明）
	context->OMSetBlendState(states->AlphaBlend(), nullptr, 0xFFFFFFFF);
	// 深度バッファの設定（深度バッファを使用しない）
	context->OMSetDepthStencilState(states->DepthNone(), 0);
	// カリングの設定（カリングしない）
	context->RSSetState(states->CullNone());

	// 入力レイアウトの設定
	context->IASetInputLayout(m_inputLayout.Get());

	// テクスチャサンプラーの設定
	ID3D11SamplerState* samplers[] = { states->LinearClamp() };
	context->PSSetSamplers(0, 1, samplers);

	//	-----体力バーの下地の描画-----
	m_basicEffect->SetTexture(m_hpTexture_a.Get());
	m_basicEffect->Apply(context);

	// 体力バーの大きさはワールド空間の座標系に対して画像サイズの150分の1にしている
	float scale = 1.0f / 150.0f;

	// 体力バーの幅と高さ
	float w = 224.0f * scale;
	float h = 32.0f * scale;

	// 頂点座標
	VertexPositionTexture v[4] = {
		{ SimpleMath::Vector3(0.0f,  0.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f) },
		{ SimpleMath::Vector3(   w,  0.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f) },
		{ SimpleMath::Vector3(   w,    -h, 0.0f), SimpleMath::Vector2(1.0f, 1.0f) },
		{ SimpleMath::Vector3(0.0f,    -h, 0.0f), SimpleMath::Vector2(0.0f, 1.0f) },
	};

	// 体力バーのセンターの位置を横方向のみ中心に調整する
	v[0].position.x -= w / 2.0f;
	v[1].position.x -= w / 2.0f;
	v[2].position.x -= w / 2.0f;
	v[3].position.x -= w / 2.0f;

	// プリミティブの描画
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(v[0], v[1], v[2], v[3]);
	m_primitiveBatch->End();

	// ----体力バー（オレンジ色のバー）の描画 ----
	m_basicEffect->SetTexture(m_hpTexture_b.Get());
	m_basicEffect->Apply(context);

	// m_hpは体力（0～100）
	float hp = m_hp / 100.0f;

	// 体力バーの下地の左上からのオフセット（配置位置）
	float off_x = 10.0f * scale;
	float off_y = 10.0f * scale;
	// 体力バーの幅と高さ
	float w2 = 204.0f * scale * hp;
	float h2 = 12.0f * scale;

	// 頂点情報
	VertexPositionTexture v2[4] = {
		{ SimpleMath::Vector3(     off_x,  0.0f - off_y, 0.0f), SimpleMath::Vector2(0.0f, 0.0f) },
		{ SimpleMath::Vector3(w2 + off_x,  0.0f - off_y, 0.0f), SimpleMath::Vector2(  hp, 0.0f) },
		{ SimpleMath::Vector3(w2 + off_x,   -h2 - off_y, 0.0f), SimpleMath::Vector2(  hp, 1.0f) },
		{ SimpleMath::Vector3(     off_x,   -h2 - off_y, 0.0f), SimpleMath::Vector2(0.0f, 1.0f) },
	};

	// 体力バーのセンターの位置を横方向のみ中心に調整する
	v2[0].position.x -= w / 2.0f;
	v2[1].position.x -= w / 2.0f;
	v2[2].position.x -= w / 2.0f;
	v2[3].position.x -= w / 2.0f;

	// プリミティブの描画
	m_primitiveBatch->Begin();
	m_primitiveBatch->DrawQuad(v2[0], v2[1], v2[2], v2[3]);
	m_primitiveBatch->End();
}

// HPを設定する
void EnemyHitPoint::SetHP(float hp)
{
	m_hp = hp;
}

// 位置を設定する
void EnemyHitPoint::SetEnemyPosition(DirectX::SimpleMath::Vector3 position)
{
	m_position = position;
}

void EnemyHitPoint::SetCameraPosition(DirectX::SimpleMath::Vector3 cameraposition)
{
	m_cameraPosition = cameraposition;
}
