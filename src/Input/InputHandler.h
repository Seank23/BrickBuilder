#pragma once
#include "./DX12Engine/Input/InputHandler.h"
#include "./DX12Engine/Input/Camera.h"
#include <wtypes.h>

namespace BrickBuilder
{
	class Camera;
	class GameContext;

	class InputHandler : public DX12Engine::InputHandler
	{
	public:
		InputHandler(GameContext& context);
		~InputHandler();

		virtual void ProcessInput(float deltaTime) override;
		virtual void HandleMouseMovement(HWND hwnd, LPARAM lParam) override;
		virtual void HandleMouseClick(HWND hwnd, LPARAM lParam) override;

		void SetWindowSize(DirectX::XMFLOAT2 size) { m_WindowSize = size; }

	private:
		void ProcessMouseClick(DX12Engine::InputCommand command, DirectX::XMVECTOR worldPosition);

		DirectX::XMFLOAT2 m_WindowSize;
		GameContext& m_GameContext;
	};
}