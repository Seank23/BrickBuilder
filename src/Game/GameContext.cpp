#include "GameContext.h"
#include "SandboxScene.h"
#include "../Input/InputHandler.h"

namespace BrickBuilder
{
	GameContext::GameContext()
	{

	}

	GameContext::~GameContext()
	{
	}

	void GameContext::Init()
	{
		m_Scene = std::make_unique<SandboxScene>();
		m_Scene->Init();

		m_InputHandler = std::make_unique<InputHandler>();
		m_InputHandler->SetCamera(m_Scene->GetCamera());
	}

	void GameContext::Update(float ts, float elapsed)
	{
		m_InputHandler->ProcessInput(ts);
		m_Scene->Update(ts, elapsed);
	}

	void GameContext::HandleWindowEvent(HWND hwnd, UINT uMsg, LPARAM lParam)
	{
		if (this == nullptr) return;
		switch (uMsg)
		{
		case WM_SIZE:
			OnResize({ (float)LOWORD(lParam), (float)HIWORD(lParam) });
			break;
		case WM_MOUSEMOVE:
			m_InputHandler->HandleMouseMovement(hwnd, lParam);
			break;
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
			m_InputHandler->HandleMouseClick(hwnd, lParam);
			break;
		}
	}

	void GameContext::OnResize(DirectX::XMFLOAT2 newSize)
	{
		if (m_InputHandler.get() != nullptr) m_InputHandler->SetWindowSize(newSize);
		if (m_Scene->GetCamera() != nullptr) m_Scene->GetCamera()->SetAspectRatio(newSize.x / newSize.y);
	}
}