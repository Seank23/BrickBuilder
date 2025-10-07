#pragma once
#include <memory>
#include "./BrickBuilderScene.h"

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

		void Init(DirectX::XMFLOAT2 windowSize);
		void Update(float ts, float elapsed);

		void HandleWindowEvent(HWND hwnd, UINT uMsg, LPARAM lParam);

		InputHandler* GetInputHandler() { return m_InputHandler.get(); }
		BrickBuilderScene* GetScene() { return m_Scene.get(); }

	private:
		void OnResize(DirectX::XMFLOAT2 newSize);

		std::unique_ptr<InputHandler> m_InputHandler;
		std::unique_ptr<BrickBuilderScene> m_Scene;
	};
}