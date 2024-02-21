#pragma once
#include "StepTimer.h"
#include "DeviceResources.h"
#include "MyLib/TransitionMask.h"

class UserResources
{
private:

	// ステップタイマー
	DX::StepTimer* m_timer;

	// デバイスリソース
	DX::DeviceResources* m_deviceResources;

	// キーボードステートトラッカー
	DirectX::Keyboard::KeyboardStateTracker* m_keyboardTracker;

	// マウスステートトラッカー
	DirectX::Mouse::ButtonStateTracker* m_mouseTracker;

	// 共通ステート
	DirectX::CommonStates* m_states;

	// 画面遷移マスクへのポインタ
	TransitionMask* m_transitionMask;

	// 勝利判定
	bool m_victoryFlag;

	// オーディオエンジン
	DirectX::AudioEngine* m_audioEngine;

public:

	// コンストラクタ
	UserResources()
		: m_timer(nullptr)
		, m_deviceResources(nullptr)
		, m_keyboardTracker(nullptr)
		, m_mouseTracker(nullptr)
		, m_states(nullptr)
		, m_transitionMask(nullptr)
		, m_audioEngine(nullptr)
		, m_victoryFlag(false)
	{
	}

	// ------------------------------------------ //
	// ステップタイマー	
	// ------------------------------------------ //

	// ステップタイマーを設定する関数
	void SetStepTimerStates(DX::StepTimer* timer) { m_timer = timer; }

	// ステップタイマーを取得する関数
	DX::StepTimer* GetStepTimer() { return m_timer; }

	// ------------------------------------------ //
	// デバイスリソース	
	// ------------------------------------------ //

	// デバイスリソースを設定する関数
	void SetDeviceResources(DX::DeviceResources* deviceResources) { m_deviceResources = deviceResources; }

	// デバイスリソースを取得する関数
	DX::DeviceResources* GetDeviceResources() { return m_deviceResources; }

	// ------------------------------------------ //
	// キーボードステートトラッカー	
	// ------------------------------------------ //

	// キーボードステートトラッカーを設定する関数
	void SetKeyboardStateTracker(DirectX::Keyboard::KeyboardStateTracker* tracker) { m_keyboardTracker = tracker; }

	// キーボードステートトラッカーを取得する関数
	DirectX::Keyboard::KeyboardStateTracker* GetKeyboardStateTracker() { return m_keyboardTracker; }

	// ------------------------------------------ //
	// マウスステートトラッカー	
	// ------------------------------------------ //

	// マウスステートトラッカーを設定する関数
	void SetMouseStateTracker(DirectX::Mouse::ButtonStateTracker* tracker) { m_mouseTracker = tracker; }

	// マウスステートトラッカーを取得する関数
	DirectX::Mouse::ButtonStateTracker* GetMouseStateTracker() { return m_mouseTracker; }

	// ------------------------------------------ //
	// 共通ステート	
	// ------------------------------------------ //

	// 共通ステートを設定する関数
	void SetCommonStates(DirectX::CommonStates* states) { m_states = states; }

	// 共通ステートを取得する関数
	DirectX::CommonStates* GetCommonStates() { return m_states; }

	// ------------------------------------------ //
	// 画面遷移マスク	
	// ------------------------------------------ //

	// 画面遷移マスクを設定する関数
	void SetTransitionMask(TransitionMask* transitionMask) { m_transitionMask = transitionMask; }

	// 画面遷移マスクを取得する関数
	TransitionMask* GetTransitionMask() { return m_transitionMask; }

	// ------------------------------------------ //
	// 勝利判定	
	// ------------------------------------------ //

	// 勝利判定を設定する関数
	void SetVictoryFlag(bool victoryFlag) { m_victoryFlag = victoryFlag; }

	// 勝利判定を取得する関数
	bool GetVictoryFlag() { return m_victoryFlag; }


	// ------------------------------------------ //
	// オーディオエンジン
	// ------------------------------------------ //

	// オーディオエンジンを設定する関数
	void SetAudioEngine(DirectX::AudioEngine* audioEngine) { m_audioEngine = audioEngine; }

	// オーディオエンジンを取得する関数
	DirectX::AudioEngine* GetAudioEngine() { return m_audioEngine; }

};

