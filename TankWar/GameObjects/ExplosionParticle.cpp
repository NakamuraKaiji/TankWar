#include "pch.h"
#include "ExplosionParticle.h"
#include "Utilities/Resources.h"
#include "MyLib/BinaryFile.h"
#include "GameParameter.h"
#include <random>

using namespace DirectX;

// インプットレイアウト	
const std::vector<D3D11_INPUT_ELEMENT_DESC>	ExplosionParticle::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0,							 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,	sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,	0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// コンストラクタ
ExplosionParticle::ExplosionParticle()
	: m_timer(0.0f)
	, m_scale(SimpleMath::Vector3::Zero)
{
	// リソースをロード
	Resources::GetInstance()->LoadResource();

	// シェーダーの作成
	CreateShader();

	// テクスチャ読み込み
	LoadTexture();
}

// テクスチャリソース読み込み
void ExplosionParticle::LoadTexture()
{
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> texture;
	texture = Resources::GetInstance()->GetParticle();

	m_texture.push_back(texture);
}

// シェーダ制作部分だけ分離した関数
void ExplosionParticle::CreateShader()
{
	// コンパイルされたシェーダファイルを読み込み
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleVS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticleGS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/ParticlePS.cso");

	// インプットレイアウトの作成
	m_graphics->GetDeviceResources()->GetD3DDevice()->CreateInputLayout(
		&INPUT_LAYOUT[0],
		static_cast<UINT>(INPUT_LAYOUT.size()),
		VSData.GetData(),
		VSData.GetSize(),
		m_inputLayout.GetAddressOf()
	);

	// 頂点シェーダ作成
	if (FAILED(m_graphics->GetDeviceResources()->GetD3DDevice()->CreateVertexShader(
		VSData.GetData(),
		VSData.GetSize(),
		NULL,
		m_vertexShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateVertexShader Failed.", NULL, MB_OK);
		return;
	}

	// ジオメトリシェーダ作成
	if (FAILED(m_graphics->GetDeviceResources()->GetD3DDevice()->CreateGeometryShader(
		GSData.GetData(),
		GSData.GetSize(),
		NULL,
		m_geometryShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreateGeometryShader Failed.", NULL, MB_OK);
		return;
	}
	// ピクセルシェーダ作成
	if (FAILED(m_graphics->GetDeviceResources()->GetD3DDevice()->CreatePixelShader(
		PSData.GetData(),
		PSData.GetSize(),
		NULL,
		m_pixelShader.ReleaseAndGetAddressOf())))
	{// エラー
		MessageBox(0, L"CreatePixelShader Failed.", NULL, MB_OK);
		return;
	}

	// シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	m_graphics->GetDeviceResources()->GetD3DDevice()->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

// 更新
void ExplosionParticle::Update(float elapsedTime)
{
	// 0.01秒ごとに生成
	m_timer += elapsedTime;
	if (m_timer >= 0.01f)
	{
		std::random_device seed;
		std::default_random_engine engine(seed());
		std::uniform_real_distribution<> dist(0, XM_2PI);
		float range = BLACK_SMOKE_RANGE;
		float rand = static_cast<float>(dist(engine));
		ParticleUtility pU(
			1.0f,
			SimpleMath::Vector3(m_position.x + range * cosf(rand), 0.5f, m_position.z + range * sinf(rand)),            // 基準座標
			SimpleMath::Vector3(0.0f, 0.5f, 0.0f),			                                                            // 速度
			SimpleMath::Vector3(0.0f, 0.5f, 0.0f), 					                                                    // 加速度
			m_scale, SimpleMath::Vector3::Zero,	                                                                        // 初期スケール、最終スケール
			SimpleMath::Color(0.41f, 0.41f, 0.41, 0.8f), SimpleMath::Color(0.0f, 0.0f, 0.0f, 1.0f)	                    // 初期カラー、  最終カラー
		);

		m_particleUtility.push_back(pU);
		m_timer = 0.0f;
	}


	//timerを渡してm_effectの更新処理を行う
	for (std::list<ParticleUtility>::iterator ite = m_particleUtility.begin(); ite != m_particleUtility.end(); ite++)
	{
		// 子クラスからfalseで消す
		if (!(ite)->Update(elapsedTime))
		{
			ite = m_particleUtility.erase(ite);
			if (ite == m_particleUtility.end()) break;
		}
	}
}

// 描画
void ExplosionParticle::Render()
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_graphics->GetCommonStates();

	DirectX::SimpleMath::Vector3 cameraDir = m_cameraTarget - m_cameraPosition;
	cameraDir.Normalize();

	m_particleUtility.sort(
		[&](ParticleUtility lhs, ParticleUtility  rhs)
		{
			//カメラ正面の距離でソート
			return cameraDir.Dot(lhs.GetPosition() - m_cameraPosition) > cameraDir.Dot(rhs.GetPosition() - m_cameraPosition);
		});
	//登録されている頂点をリセット
	m_vertices.clear();
	for (auto& li : m_particleUtility)
	{
		if (cameraDir.Dot(li.GetPosition() - m_cameraPosition) < 0.0f) {
			//内積がマイナスの場合はカメラの後ろなので表示する必要なし
			continue;
		}
		VertexPositionColorTexture vPCT;
		vPCT.position = XMFLOAT3(li.GetPosition());
		vPCT.color = XMFLOAT4(li.GetNowColor());
		vPCT.textureCoordinate = XMFLOAT2(li.GetNowScale().x, 0.0f);

		m_vertices.push_back(vPCT);
	}

	//表示する点がない場合は描画を終わる
	if (m_vertices.empty())
	{
		return;
	}

	//シェーダーに渡す追加のバッファを作成する。(ConstBuffer）
	ConstBuffer cbuff;
	cbuff.matView = m_graphics->GetViewMatrix().Transpose();
	cbuff.matProj = m_graphics->GetProjectionMatrix().Transpose();
	cbuff.matWorld = m_billboard.Transpose();
	cbuff.Diffuse = SimpleMath::Vector4(1, 1, 1, 1);

	//受け渡し用バッファの内容更新(ConstBufferからID3D11Bufferへの変換）
	context->UpdateSubresource(m_CBuffer.Get(), 0, NULL, &cbuff, 0, 0);

	//シェーダーにバッファを渡す
	ID3D11Buffer* cb[1] = { m_CBuffer.Get() };
	context->VSSetConstantBuffers(0, 1, cb);
	context->GSSetConstantBuffers(0, 1, cb);
	context->PSSetConstantBuffers(0, 1, cb);

	//画像用サンプラーの登録
	ID3D11SamplerState* sampler[1] = { states->LinearWrap() };
	context->PSSetSamplers(0, 1, sampler);

	//半透明描画指定
	ID3D11BlendState* blendstate = states->NonPremultiplied();

	// 透明判定処理
	context->OMSetBlendState(blendstate, nullptr, 0xFFFFFFFF);

	// 深度バッファに書き込み参照する
	context->OMSetDepthStencilState(states->DepthDefault(), 0);

	// カリングは左周り
	context->RSSetState(states->CullNone());

	//シェーダをセットする
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	context->GSSetShader(m_geometryShader.Get(), nullptr, 0);
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	//ピクセルシェーダにテクスチャを登録する。
	for (int i = 0; i < m_texture.size(); i++)
	{
		context->PSSetShaderResources(i, 1, m_texture[i].GetAddressOf());
	}

	//インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	// 板ポリゴンを描画
	m_graphics->GetPrimitiveBatch()->Begin();
	m_graphics->GetPrimitiveBatch()->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &m_vertices[0], m_vertices.size());
	m_graphics->GetPrimitiveBatch()->End();

	//シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);
}

// ビルボード
void ExplosionParticle::CreateBillboard(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 up)
{
	m_billboard =
		SimpleMath::Matrix::CreateBillboard(SimpleMath::Vector3::Zero, eye - target, up);

	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;
	rot._11 = -1;
	rot._33 = -1;

	m_cameraPosition = eye;
	m_cameraTarget = target;
	m_billboard = rot * m_billboard;
}
