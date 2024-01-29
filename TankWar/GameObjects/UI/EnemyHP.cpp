#include "pch.h"
#include "EnemyHP.h"
#include "MyLib/BinaryFile.h"


using namespace DirectX;

// インプットレイアウト	
const std::vector<D3D11_INPUT_ELEMENT_DESC>	EnemyHP::INPUT_LAYOUT =
{
	{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, sizeof(SimpleMath::Vector3), D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(SimpleMath::Vector3) + sizeof(SimpleMath::Vector4), D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

// コンストラクタ
EnemyHP::EnemyHP()
	: m_texture(nullptr)
	, m_textureHeight(0)
	, m_textureWidth(0)
	, m_res(nullptr)
	, m_ratio(1.0f)
	, m_position{}
{
	//シェーダーの作成
	CreateShader();

	//画像の読み込み
	LoadTexture(L"Resources/dds/HP.dds");
}

// テクスチャリソース読み込み
void EnemyHP::LoadTexture(const wchar_t* path)
{
	DirectX::CreateDDSTextureFromFile(
		m_graphics->GetDeviceResources()->GetD3DDevice(),
		path, 
		m_res.ReleaseAndGetAddressOf(), 
		m_texture.ReleaseAndGetAddressOf()
	);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex;
	DX::ThrowIfFailed(m_res.As(&tex));

	// 画像サイズを取得
	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc);
	m_textureWidth = desc.Width;
	m_textureHeight = desc.Height;
}

// シェーダ制作部分だけ分離した関数
void EnemyHP::CreateShader()
{
	// コンパイルされたシェーダーファイルを読み込む
	BinaryFile VSData = BinaryFile::LoadFile(L"Resources/Shaders/HP_VS.cso");
	BinaryFile GSData = BinaryFile::LoadFile(L"Resources/Shaders/HP_GS.cso");
	BinaryFile PSData = BinaryFile::LoadFile(L"Resources/Shaders/HP_PS.cso");

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

	//シェーダーにデータを渡すためのコンスタントバッファ生成
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(ConstBuffer);
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	m_graphics->GetDeviceResources()->GetD3DDevice()->CreateBuffer(&bd, nullptr, &m_CBuffer);
}

// 更新
bool EnemyHP::Update(float elapsedTime)
{
	UNREFERENCED_PARAMETER(elapsedTime);

	return true;
}

// 描画
void EnemyHP::Render()
{
	auto context = m_graphics->GetDeviceResources()->GetD3DDeviceContext();
	auto states = m_graphics->GetCommonStates();

	m_position = SimpleMath::Vector3(m_position.x, 1.0f, m_position.z);

	// 頂点情報
	VertexPositionColorTexture vertex[1] =
	{
		VertexPositionColorTexture(m_position
		, SimpleMath::Vector4(0.0f, 0.0f, static_cast<float>(m_textureWidth), static_cast<float>(m_textureHeight))
		, SimpleMath::Vector2(m_ratio, 0))
	};

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
	context->PSSetShaderResources(0, 1, m_texture.GetAddressOf());

	//インプットレイアウトの登録
	context->IASetInputLayout(m_inputLayout.Get());

	// 描画
	m_graphics->GetPrimitiveBatch()->Begin();
	m_graphics->GetPrimitiveBatch()->Draw(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST, &vertex[0], 1);
	m_graphics->GetPrimitiveBatch()->End();

	//シェーダの登録を解除しておく
	context->VSSetShader(nullptr, nullptr, 0);
	context->GSSetShader(nullptr, nullptr, 0);
	context->PSSetShader(nullptr, nullptr, 0);

}

// ビルボード
void EnemyHP::CreateBillboard(DirectX::SimpleMath::Vector3 target, DirectX::SimpleMath::Vector3 eye, DirectX::SimpleMath::Vector3 up)
{
	m_billboard =
		//SimpleMath::Matrix::CreateBillboard(target, eye , up);
		SimpleMath::Matrix::CreateBillboard(SimpleMath::Vector3::Zero, eye - target, up);

	SimpleMath::Matrix rot = SimpleMath::Matrix::Identity;
	rot._11 = -1;
	rot._33 = -1;

	m_billboard = rot * m_billboard;

}

