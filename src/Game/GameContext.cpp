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

	void GameContext::Init(DirectX::XMFLOAT2 windowSize)
	{
		m_Scene = std::make_unique<SandboxScene>(*this);
		m_Scene->Init();

		m_InputHandler = std::make_unique<InputHandler>(*this);
		m_InputHandler->SetCamera(m_Scene->GetCamera());

		OnResize(windowSize);
	}

	void GameContext::Update(float ts, float elapsed)
	{
		m_InputHandler->ProcessInput(ts);
		m_Scene->Update(ts, elapsed);
	}

	void GameContext::HandleWindowEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		case WM_MOUSEWHEEL:
			m_InputHandler->HandleMouseWheel(hwnd, wParam);
		}
	}

	void GameContext::LoadMesh(const std::string& meshName, std::shared_ptr<DX12Engine::Mesh> mesh)
	{
		m_BrickMeshes[meshName] = mesh;
		m_MeshIterator = m_BrickMeshes.begin();
	}

	void GameContext::LoadMaterial(const std::string& materialName, std::shared_ptr<DX12Engine::PBRMaterial> material)
	{
		m_BrickMaterials[materialName] = material;
		m_MaterialIterator = m_BrickMaterials.begin();
	}

	void GameContext::CycleMaterial(bool forward)
	{
		if (forward)
		{
			m_MaterialIterator++;
			if (m_MaterialIterator == m_BrickMaterials.end())
				m_MaterialIterator = m_BrickMaterials.begin();
		}
		else
		{
			if (m_MaterialIterator == m_BrickMaterials.begin())
				m_MaterialIterator = m_BrickMaterials.end();
			m_MaterialIterator--;
		}
	}

	void GameContext::OnResize(DirectX::XMFLOAT2 newSize)
	{
		if (m_InputHandler.get() != nullptr) m_InputHandler->SetWindowSize(newSize);
		if (m_Scene->GetCamera() != nullptr) m_Scene->GetCamera()->SetAspectRatio(newSize.x / newSize.y);
	}
}