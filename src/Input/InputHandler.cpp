#include "InputHandler.h"
#include <windowsx.h>
#include <cstdio>
#include "DX12Engine/Input/InputController.h"

namespace BrickBuilder
{
	InputHandler::InputHandler()
		: DX12Engine::InputHandler()
	{
		m_CommandMap = {
			{ DX12Engine::InputCommand::MoveForward,  VK_UP },
			{ DX12Engine::InputCommand::MoveBackward, VK_DOWN },
			{ DX12Engine::InputCommand::MoveLeft,     VK_LEFT },
			{ DX12Engine::InputCommand::MoveRight,    VK_RIGHT },
			{ DX12Engine::InputCommand::MoveUp,       VK_RSHIFT },
			{ DX12Engine::InputCommand::MoveDown,     VK_RCONTROL },
			{ DX12Engine::InputCommand::Pan,		  VK_RBUTTON },
			{ DX12Engine::InputCommand::Interact,	  VK_LBUTTON },
		};
	}

	InputHandler::~InputHandler()
	{
	}

	void InputHandler::ProcessInput(float deltaTime)
	{
		DX12Engine::InputHandler::ProcessInput(deltaTime);
	}

	void InputHandler::HandleMouseMovement(HWND hwnd, LPARAM lParam)
	{
		DX12Engine::InputHandler::HandleMouseMovement(hwnd, lParam);
	}

	void InputHandler::HandleMouseClick(HWND hwnd, LPARAM lParam)
	{
		DX12Engine::InputHandler::HandleMouseClick(hwnd, lParam);
	}
}