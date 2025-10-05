#include "InputHandler.h"
#include <windowsx.h>
#include <cstdio>
#include "DirectXMath.h"
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

		float ndcX = (2.0f * m_LastMouseX) / m_WindowSize.x - 1.0f;
		float ndcY = 1.0f - (2.0f * m_LastMouseY) / m_WindowSize.y;
		DirectX::XMVECTOR nearPoint = DirectX::XMVectorSet(ndcX, ndcY, 0.0f, 1.0f);
		DirectX::XMVECTOR farPoint = DirectX::XMVectorSet(ndcX, ndcY, 1.0f, 1.0f);
		DirectX::XMMATRIX invViewProj = DirectX::XMMatrixInverse(nullptr, m_Camera->GetViewMatrix() * m_Camera->GetProjectionMatrix());
		DirectX::XMVECTOR nearWorld = DirectX::XMVector3TransformCoord(nearPoint, invViewProj);
		DirectX::XMVECTOR farWorld = DirectX::XMVector3TransformCoord(farPoint, invViewProj);
		DirectX::XMVECTOR rayOrigin = nearWorld;
		DirectX::XMVECTOR rayDirection = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(farWorld, nearWorld));
		float t = -DirectX::XMVectorGetY(rayOrigin) / DirectX::XMVectorGetY(rayDirection);
		DirectX::XMVECTOR intersectPoint = DirectX::XMVectorAdd(rayOrigin, DirectX::XMVectorScale(rayDirection, t));
		std::printf("Intersect Point: (%.2f, %.2f, %.2f)\n",
			DirectX::XMVectorGetX(intersectPoint),
			DirectX::XMVectorGetY(intersectPoint),
			DirectX::XMVectorGetZ(intersectPoint));
	}
}