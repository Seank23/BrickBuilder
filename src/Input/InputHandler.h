#pragma once
#include "./DX12Engine/Input/InputHandler.h"
#include "./DX12Engine/Input/Camera.h"
#include <wtypes.h>

namespace BrickBuilder
{
	class Camera;

	class InputHandler : public DX12Engine::InputHandler
	{
	public:
		InputHandler();
		~InputHandler();

		virtual void ProcessInput(float deltaTime) override;
		virtual void HandleMouseMovement(HWND hwnd, LPARAM lParam) override;
		virtual void HandleMouseClick(HWND hwnd, LPARAM lParam) override;
	};
}