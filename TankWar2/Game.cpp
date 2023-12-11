//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include "Scenes/TitleScene.h"
#include "Scenes/PlayScene.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept(false)
    : m_fullscreen(FALSE)

{
    m_deviceResources = std::make_unique<DX::DeviceResources>();
    // TODO: Provide parameters for swapchain format, depth/stencil format, and backbuffer count.
    //   Add DX::DeviceResources::c_AllowTearing to opt-in to variable rate displays.
    //   Add DX::DeviceResources::c_EnableHDR for HDR10 display.
    m_deviceResources->RegisterDeviceNotify(this);
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
    m_deviceResources->SetWindow(window, width, height);

    m_deviceResources->CreateDeviceResources();
    CreateDeviceDependentResources();

    m_deviceResources->CreateWindowSizeDependentResources();
    CreateWindowSizeDependentResources();

    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);

    // �����N���V�[��
    m_sceneManager->SetScene<PlayScene>();
}

#pragma region Frame Update
// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{
    // TODO: Add your game logic here.

    // �L�[�{�[�h�X�e�[�g�g���b�J�[�̍X�V
    m_keyboardTracker.Update(Keyboard::Get().GetState());
    // �}�E�X�X�e�[�g�g���b�J�[�̍X�V
    m_mouseTracker.Update(Mouse::Get().GetState());

    // �V�[���̍X�V
    m_sceneManager->Update(timer);

}
#pragma endregion

#pragma region Frame Render
// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();

    m_deviceResources->PIXBeginEvent(L"Render");
    auto context = m_deviceResources->GetD3DDeviceContext();

    // TODO: Add your rendering code here.
    context;

    // �V�[���̕`��
    m_sceneManager->Render();
    // fps�̕\��
    //std::wostringstream oss;
    //oss << "fps:" << m_timer.GetFramesPerSecond();
    //m_debugFont->AddString(oss.str().c_str(), SimpleMath::Vector2(0.0f, 0.0f));

    // �f�o�b�O�p������̕`��
    m_debugFont->Render(m_states.get());

    m_deviceResources->PIXEndEvent();

    // Show the new frame.
    m_deviceResources->Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    m_deviceResources->PIXBeginEvent(L"Clear");

    // Clear the views.
    auto context = m_deviceResources->GetD3DDeviceContext();
    auto renderTarget = m_deviceResources->GetRenderTargetView();
    auto depthStencil = m_deviceResources->GetDepthStencilView();

    context->ClearRenderTargetView(renderTarget, Colors::CornflowerBlue);
    context->ClearDepthStencilView(depthStencil, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    context->OMSetRenderTargets(1, &renderTarget, depthStencil);

    // Set the viewport.
    auto const viewport = m_deviceResources->GetScreenViewport();
    context->RSSetViewports(1, &viewport);

    m_deviceResources->PIXEndEvent();
}
#pragma endregion

#pragma region Message Handlers
// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowMoved()
{
    auto const r = m_deviceResources->GetOutputSize();
    m_deviceResources->WindowSizeChanged(r.right, r.bottom);

    // �t���X�N���[�������ׂ�
    BOOL fullscreen = FALSE;
    m_deviceResources->GetSwapChain()->GetFullscreenState(&fullscreen, nullptr);
    // �t���X�N���[������������Ă��܂������̏���
    if (m_fullscreen != fullscreen)
    {
        m_fullscreen = fullscreen;
        // ResizeBuffers�֐����Ăяo��
        m_deviceResources->CreateWindowSizeDependentResources();
    }

}

void Game::OnDisplayChange()
{
    m_deviceResources->UpdateColorSpace();
}

void Game::OnWindowSizeChanged(int width, int height)
{
    if (!m_deviceResources->WindowSizeChanged(width, height))
        return;

    CreateWindowSizeDependentResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    width = 1280;
    height = 720;
}

//  ��ʃ��[�h��ݒ肷��֐��iTRUE : �t���X�N���[���j
void Game::SetFullscreenState(BOOL value)
{
    m_fullscreen = value;
    m_deviceResources->GetSwapChain()->SetFullscreenState(m_fullscreen, nullptr);
    if (value) m_deviceResources->CreateWindowSizeDependentResources();
}

#pragma endregion

#pragma region Direct3D Resources
// These are the resources that depend on the device.
void Game::CreateDeviceDependentResources()
{
    auto device = m_deviceResources->GetD3DDevice();
    auto context = m_deviceResources->GetD3DDeviceContext();

    // ���ʃX�e�[�g�I�u�W�F�N�g�̍쐬
    m_states = std::make_unique<CommonStates>(device);
    // �f�o�b�O�p������\���I�u�W�F�N�g�̍쐬
    m_debugFont = std::make_unique<Imase::DebugFont>(device, context, L"Resources/Font/SegoeUI_18.spritefont");

    // ���[�U�[���\�[�X�̍쐬
    if (!m_userResources) m_userResources = std::make_unique<UserResources>();
    // �V�[���}�l�[�W���[�̍쐬
    if (!m_sceneManager) m_sceneManager = std::make_unique<Imase::SceneManager<UserResources>>(m_userResources.get());

    // �V�[���֓n�����[�U�[���\�[�X�̐ݒ�i�����Őݒ肵�Ă��������j
    m_userResources->SetDeviceResources(m_deviceResources.get());
    m_userResources->SetCommonStates(m_states.get());
    m_userResources->SetStepTimerStates(&m_timer);
    m_userResources->SetDebugFont(m_debugFont.get());
    m_userResources->SetKeyboardStateTracker(&m_keyboardTracker);
    m_userResources->SetMouseStateTracker(&m_mouseTracker);

    // �V�[���̃f�o�C�X�Ɉˑ����鏉�������s��
    m_sceneManager->CreateDeviceDependentResources();
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateWindowSizeDependentResources()
{
    // TODO: Initialize windows-size dependent objects here.

        // �V�[���̃E�B���h�E�̃T�C�Y���ύX���ꂽ���ɌĂ΂�鏈��
    m_sceneManager->CreateWindowSizeDependentResources();
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    // �V�[���̃f�o�C�X���X�g���̏���
    m_sceneManager->OnDeviceLost();
}

void Game::OnDeviceRestored()
{
    CreateDeviceDependentResources();

    CreateWindowSizeDependentResources();
}
#pragma endregion
