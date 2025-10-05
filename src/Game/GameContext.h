#pragma once
#include <memory>
#include "DX12Engine/Entity/Scene.h"

namespace BrickBuilder
{
	class InputHandler;
	class Camera;
	class LightBuffer;

	class GameContext
	{
	public:
		GameContext();
		~GameContext();

		void Init();
		void Update(float ts, float elapsed);

		void HandleWindowEvent(HWND hwnd, UINT uMsg, LPARAM lParam);

		InputHandler* GetInputHandler() { return m_InputHandler.get(); }
		DX12Engine::Scene* GetScene() { return m_Scene.get(); }

	private:
		void OnResize(DirectX::XMFLOAT2 newSize);

		std::unique_ptr<InputHandler> m_InputHandler;
		std::unique_ptr<DX12Engine::Scene> m_Scene;
	};
}