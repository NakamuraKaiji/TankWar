#pragma once

#include "ImaseLib/ResourceManager.h"
#include "CollisionManager.h"
#include "InfoWindow/InfoWindow.h"

// ゲームに必要なリソース
struct GameResources
{
	// リソースマネージャーへのポインタ
	Imase::ResourceManager* resourceManager;

	// ベーシックエフェクト（エフェクト用）
	DirectX::BasicEffect* pBasicEffect;

	// プリミティブバッチ（エフェクト用）
	DirectX::PrimitiveBatch<DirectX::VertexPositionColorTexture>* pPrimitiveBatch;

	// 入力レイアウト（エフェクト用）
	ID3D11InputLayout* m_pInputLayout;

	// デバイスコンテキストへのポインタ
	ID3D11DeviceContext* pContext;

	// 共通ステートへのポインタ
	DirectX::CommonStates* pStates;

	// スプライトバッチへのポインタ
	DirectX::SpriteBatch* pSpriteBatch;

	// ビュー行列へのポインタ
	DirectX::SimpleMath::Matrix* pView;

	// 射影行列へのポインタ
	DirectX::SimpleMath::Matrix* pProj;

	// 衝突判定用オブジェクト
	CollisionManager* pCollisionManager;

	// 情報ウインドウへのポインタ
	InfoWindow* pInfoWindow;

	// 得点を加算する関数
	std::function<void(int score)> addScore;
};

