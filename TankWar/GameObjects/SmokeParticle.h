//*********************************************************************
//			概要	：煙のパーティクルクラス
// 
//			制作日	：1月21日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "ParticleUtility.h"
#include <list>
#include <vector>


class SmokeParticle
{
public:
	//データ受け渡し用コンスタントバッファ(送信側)
	struct ConstBuffer
	{
		DirectX::SimpleMath::Matrix		matWorld;
		DirectX::SimpleMath::Matrix		matView;
		DirectX::SimpleMath::Matrix		matProj;
		DirectX::SimpleMath::Vector4	Diffuse;
	};

	// グラフィックスへのポインタ
	Graphics* m_graphics = Graphics::GetInstance();

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// テクスチャハンドル
	std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_texture;

	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;

	std::vector<DirectX::VertexPositionColorTexture> m_vertices;
	std::list<ParticleUtility> m_particleUtility;

private:

	// ビルボード
	DirectX::SimpleMath::Matrix m_billboard;

	// 表示位置
	DirectX::SimpleMath::Vector3 m_position;

	DirectX::SimpleMath::Vector3 m_cameraPosition;
	DirectX::SimpleMath::Vector3 m_cameraTarget;
	
	// タイマー
	float m_timer;


public:

	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// コンストラクタ
	SmokeParticle();

	// テクスチャリソース読み込み
	void LoadTexture();

	// 更新
	void Update(float elapsedTime);

	// 描画
	void Render();

	// ビルボード関数
	void CreateBillboard(
		DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 eye,
		DirectX::SimpleMath::Vector3 up);

public:

	// 位置取得
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }

	// 位置設定
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }

private:

	// シェーダ制作部分だけ分離した関数
	void CreateShader();

};