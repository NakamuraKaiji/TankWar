//*********************************************************************
//			概要	：サウンドマネージャーを生成するクラス
// 
//			制作日	：1月29日
// 
//			製作者	：Kaiji Nakamura
//*********************************************************************
#pragma once
#include "MyLib/SoundManager.h"

class SoundCreate
{
private:

	// サウンドマネージャー
	std::unique_ptr<SoundManager> m_soundManager;

	// サウンド生成
	static std::unique_ptr<SoundCreate> m_soundCreate;

	SoundCreate()
	{
		// サウンドマネージャーの生成
		m_soundManager = std::make_unique<SoundManager>();
	}

public:

	// SoundCreateクラスのインスタンスを取得する
	static SoundCreate* const GetInstance();

	// サウンドマネージャーを取得する
	SoundManager* GetSoundManager() { return m_soundManager.get(); }
};