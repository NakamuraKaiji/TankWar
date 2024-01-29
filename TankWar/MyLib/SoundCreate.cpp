#include "pch.h"
#include "SoundCreate.h"

std::unique_ptr<SoundCreate> SoundCreate::m_soundCreate = nullptr;

SoundCreate* const SoundCreate::GetInstance()
{
	// SoundCreateがnullptrなら処理する
	if (m_soundCreate == nullptr)
	{
		// SoundCreateクラスのインスタンスを生成する
		m_soundCreate.reset(new SoundCreate());
	}

	// SoundManagerクラスのインスタンスを返す
	return m_soundCreate.get();
}