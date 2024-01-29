//*********************************************************************
//			概要	：敵のHPクラス
// 
//			制作日	：1月15日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "MyLib/TaskManager.h"

class EnemyHP : public Task
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

private:

	// グラフィックスへのポインタ
	Graphics* m_graphics = Graphics::GetInstance();

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_CBuffer;

	// 入力レイアウト
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;

	// テクスチャハンドル
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_texture;

	Microsoft::WRL::ComPtr<ID3D11Resource> m_res;

	// 頂点シェーダ
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	// ピクセルシェーダ
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	// ジオメトリシェーダ
	Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_geometryShader;


private:

	// ビルボード
	DirectX::SimpleMath::Matrix m_billboard;

	// 表示位置
	DirectX::SimpleMath::Vector3 m_position;

	// HPの割合
	float m_ratio;

	//	画像サイズ
	int m_textureWidth, m_textureHeight;

	DirectX::Keyboard::KeyboardStateTracker m_tracker;


public:
	
	static const std::vector<D3D11_INPUT_ELEMENT_DESC> INPUT_LAYOUT;

	// コンストラクタ
	EnemyHP();

	// テクスチャリソース読み込み
	void LoadTexture(const wchar_t* path);

	// 更新
	bool Update(float elapsedTime) override;

	// 描画
	void Render() override;

	// ビルボード
	void CreateBillboard(
		DirectX::SimpleMath::Vector3 target,
		DirectX::SimpleMath::Vector3 eye,
		DirectX::SimpleMath::Vector3 up);

public:

	// 位置取得
	DirectX::SimpleMath::Vector3 GetPosition() { return m_position; }

	// 位置設定
	void SetPosition(DirectX::SimpleMath::Vector3 position) { m_position = position; }

	// HPの割合を取得
	float GetRatio() { return m_ratio; }

	// HPの割合を設定
	void SetRatio(float ratio) { m_ratio = ratio; }

private:
	// シェーダ制作部分だけ分離した関数
	void CreateShader();

};