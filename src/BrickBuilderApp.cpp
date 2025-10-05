#include "BrickBuilderApp.h"
#include <windowsx.h>

#include "DX12Engine/Resources/Shader.h"
#include "DX12Engine/IO/ModelLoader.h"
#include "DX12Engine/Resources/Mesh.h"
#include "DX12Engine/Entity/RenderComponent.h"
#include "DX12Engine/IO/TextureLoader.h"
#include "DX12Engine/Resources/Texture.h"
#include "DX12Engine/Rendering/GPUUploader.h"
#include "DX12Engine/Resources/Materials/PBRMaterial.h"
#include "DX12Engine/Resources/Light.h"
#include "DX12Engine/Resources/ResourceManager.h"
#include "DX12Engine/Rendering/RenderPass/RenderPass.h"
#include "DX12Engine/Rendering/RenderPipelineConfig.h"

namespace BrickBuilder
{
	BrickBuilder::BrickBuilder()
		: Application()
	{
	}

	BrickBuilder::~BrickBuilder()
	{
	}

	void BrickBuilder::Init(std::shared_ptr<DX12Engine::RenderContext> renderContext, DirectX::XMFLOAT2 windowSize)
	{
		m_RenderContext = renderContext;
		m_Renderer = std::make_unique<DX12Engine::Renderer>(m_RenderContext);
		DX12Engine::ResourceManager::GetInstance().AddShader("UIGrid_PS", DX12Engine::ResourceManager::GetShaderPath("UIGrid_PS.hlsl"), DX12Engine::ShaderType::Pixel);

		m_GameContext = std::make_unique<GameContext>();
		m_GameContext->Init();

		DX12Engine::RenderPipelineConfig pipelineConfig;
		DX12Engine::RenderPassConfig geometryConfig;
		geometryConfig.Type = DX12Engine::RenderPassType::Geometry;
		std::vector<DX12Engine::RenderTargetType> gBufferTypes{
			DX12Engine::RenderTargetType::Albedo,
			DX12Engine::RenderTargetType::WorldNormal,
			DX12Engine::RenderTargetType::ObjectNormal,
			DX12Engine::RenderTargetType::Material,
			DX12Engine::RenderTargetType::Position,
			DX12Engine::RenderTargetType::Depth
		};

		std::vector<DX12Engine::RenderTargetType> shadowBufferTypes{
			DX12Engine::RenderTargetType::Depth
		};
		std::vector<DX12Engine::RenderTargetType> cubeShadowBufferTypes{
			DX12Engine::RenderTargetType::Depth
		};

		std::vector<DX12Engine::RenderTargetType> uiGBufferTypes{ DX12Engine::RenderTargetType::Depth };
		std::vector<DX12Engine::Texture*> lightingExternalTextures{ 
			m_GameContext->GetScene()->GetSkyboxCubemap(),
			m_GameContext->GetScene()->GetSkyboxIrradiance()
		};
		DX12Engine::RenderPassConfig lightingConfig;
		lightingConfig.Type = DX12Engine::RenderPassType::Lighting;
		lightingConfig.InputResources[DX12Engine::InputResourceType::ExternalTextures] = &lightingExternalTextures;
		lightingConfig.InputResources[DX12Engine::InputResourceType::RenderTargets_Geometry] = &gBufferTypes;
		lightingConfig.InputResources[DX12Engine::InputResourceType::RenderTargets_ShadowMap] = &shadowBufferTypes;
		lightingConfig.InputResources[DX12Engine::InputResourceType::RenderTargets_CubeShadowMap] = &cubeShadowBufferTypes;

		std::vector<DX12Engine::RenderTargetType> uiLightingTypes{ DX12Engine::RenderTargetType::Composite };
		std::string gridShader = "UIGrid_PS";
		DX12Engine::RenderPassConfig uiConfig;
		uiConfig.Type = DX12Engine::RenderPassType::UI;
		uiConfig.InputResources[DX12Engine::InputResourceType::PixelShader] = &gridShader;
		uiConfig.InputResources[DX12Engine::InputResourceType::RenderTargets_Geometry] = &uiGBufferTypes;
		uiConfig.InputResources[DX12Engine::InputResourceType::RenderTargets_Lighting] = &uiLightingTypes;

		pipelineConfig.Passes.push_back(geometryConfig);
		pipelineConfig.Passes.push_back(lightingConfig);
		pipelineConfig.Passes.push_back(uiConfig);
		m_RenderPipeline = m_Renderer->CreateRenderPipeline(pipelineConfig);

		m_Renderer->SetCurrentScene(m_GameContext->GetScene());
	}

	void BrickBuilder::Update(float ts, float elapsed)
	{
		m_GameContext->Update(ts, elapsed);
		m_Renderer->ExecutePipeline(m_RenderPipeline);
	}

	void BrickBuilder::HandleWindowEvent(HWND hwnd, UINT uMsg, LPARAM lParam)
	{
		if (this == nullptr) return;
		switch (uMsg)
		{
		case WM_SIZE:
			OnResize({ (float)LOWORD(lParam), (float)HIWORD(lParam) });
			break;
		}
		m_GameContext->HandleWindowEvent(hwnd, uMsg, lParam);
	}

	void BrickBuilder::OnResize(DirectX::XMFLOAT2 newSize)
	{
		if (m_RenderContext != nullptr) m_RenderContext->SetWindowSize({ (int)newSize.x, (int)newSize.y });
	}
}
