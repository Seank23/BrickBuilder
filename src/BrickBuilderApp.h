#pragma once
#include <memory>

#include "./DX12Engine/Application.h"
#include "./DX12Engine/Input/Camera.h"
#include "DX12Engine/Rendering/Buffers/LightBuffer.h"
#include "DX12Engine/Rendering/Renderer.h"
#include "DX12Engine/Rendering/RenderContext.h"
#include "DX12Engine/Entity/GameObject.h"
#include "DX12Engine/Physics/PhysicsEngine.h"
#include "Input/InputHandler.h"

namespace BrickBuilder
{
	class BrickBuilder : public DX12Engine::Application
	{

	public:
		BrickBuilder();
		~BrickBuilder();

		virtual void Init(std::shared_ptr<DX12Engine::RenderContext> renderContext, DirectX::XMFLOAT2 windowSize) override;
		virtual void Update(float ts, float elapsed) override;

		virtual void HandleWindowEvent(HWND hwnd, UINT uMsg, LPARAM lParam) override;

	private:
		void OnResize(DirectX::XMFLOAT2 newSize);

		std::unique_ptr<InputHandler> m_InputHandler;

		std::unique_ptr<DX12Engine::Camera> m_Camera;
		std::unique_ptr<DX12Engine::LightBuffer> m_LightBuffer;

		std::unique_ptr<DX12Engine::Renderer> m_Renderer;
		DX12Engine::RenderPipeline m_RenderPipeline;

		DX12Engine::GameObjectContainer m_SceneObjects;
	};
}

