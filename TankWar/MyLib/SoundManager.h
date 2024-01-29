//*********************************************************************
//			概要	：サウンドを管理するクラス
// 
//			制作日	：1月29日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include <unordered_map>
#include <string>

class SoundManager
{
private:

	// サウンドキャッシュ
	using SoundEffectCache = std::unordered_map<std::wstring, std::unique_ptr<DirectX::SoundEffect>>;
	SoundEffectCache m_soundEffectCache;

public:

	// コンストラクタ
	SoundManager();

	// サウンドエフェクトインスタンス作成関数
	DirectX::SoundEffect* CreateSoundEffectInstance(const wchar_t* name, DirectX::AudioEngine* audioEngine);
};