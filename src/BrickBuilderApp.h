#pragma once
#include <memory>

#include "./DX12Engine/Application.h"
#include "DX12Engine/Rendering/Renderer.h"
#include "DX12Engine/Rendering/RenderContext.h"
#include "DX12Engine/Physics/PhysicsEngine.h"
#include "Game/GameContext.h"

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

		std::unique_ptr<GameContext> m_GameContext;

		std::unique_ptr<DX12Engine::Renderer> m_Renderer;
		DX12Engine::RenderPipeline m_RenderPipeline;
	};
}

