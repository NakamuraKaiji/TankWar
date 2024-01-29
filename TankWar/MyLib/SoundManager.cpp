#include "pch.h"
#include "SoundManager.h"

using namespace DirectX;

// コンストラクタ
SoundManager::SoundManager()
{
}

// サウンドエフェクトインスタンス作成関数
DirectX::SoundEffect* SoundManager::CreateSoundEffectInstance(const wchar_t* name, DirectX::AudioEngine* audioEngine)
{
    auto it = m_soundEffectCache.find(name);

    // キャッシュに存在しなかった
    if (it == m_soundEffectCache.end())
    {
        // サウンドエフェクトの作成
        std::unique_ptr<DirectX::SoundEffect> sound = std::make_unique<DirectX::SoundEffect>(audioEngine, name);
        SoundEffectCache::value_type v(name, std::move(sound));
        // キャッシュに格納する
        m_soundEffectCache.insert(std::move(v));
    }

    return m_soundEffectCache.at(name).get();
}
