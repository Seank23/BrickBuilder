#include "ClientApplication.h"
#include <windowsx.h>

#include "DX12Engine/Resources/Shader.h"
#include "DX12Engine/IO/ModelLoader.h"
#include "DX12Engine/Resources/Mesh.h"
#include "DX12Engine/Entity/RenderComponent.h"
#include "DX12Engine/Entity/PhysicsComponent.h"
#include "DX12Engine/IO/TextureLoader.h"
#include "DX12Engine/Resources/Texture.h"
#include "DX12Engine/Rendering/GPUUploader.h"
#include "DX12Engine/Resources/Materials/BasicMaterial.h"
#include "DX12Engine/Resources/Materials/PBRMaterial.h"
#include "DX12Engine/Rendering/PipelineStateBuilder.h"
#include "DX12Engine/Rendering/RootSignatureBuilder.h"
#include "DX12Engine/Resources/Light.h"
#include "DX12Engine/Resources/RenderTexture.h"
#include "DX12Engine/Resources/ResourceManager.h"
#include "DX12Engine/Rendering/RenderPass/ShadowMapRenderPass.h"
#include "DX12Engine/Rendering/RenderPass/GeometryRenderPass.h"
#include "DX12Engine/Rendering/RenderPass/LightingRenderPass.h"
#include "DX12Engine/Rendering/RenderPipelineConfig.h"

ClientApplication::ClientApplication()
	: Application()
{
}

ClientApplication::~ClientApplication()
{
}

void ClientApplication::Init(std::shared_ptr<DX12Engine::RenderContext> renderContext, DirectX::XMFLOAT2 windowSize)
{
	m_RenderContext = renderContext;
	m_Renderer = std::make_unique<DX12Engine::Renderer>(m_RenderContext);
	DX12Engine::ResourceManager::GetInstance().AddShader("UIGrid_PS", DX12Engine::ResourceManager::GetShaderPath("UIGrid_PS.hlsl"), "pixel");

	m_Camera = std::make_unique<DX12Engine::Camera>(windowSize.x / windowSize.y, 0.1f, 100.0f);
	m_Camera->SetPosition({ -5.0f, 1.0f, 3.0f });
	m_Camera->SetRotation(0.0f, -30.0f);
	m_Renderer->SetCamera(m_Camera.get());

	m_LightBuffer = std::make_unique<DX12Engine::LightBuffer>();
	std::shared_ptr<DX12Engine::Light> sunLight = std::make_shared<DX12Engine::Light>();
	sunLight->SetType((int)DX12Engine::LightType::Directional);
	sunLight->SetDirection({ 0.45f, -0.577f, 0.577f });
	sunLight->SetIntensity(5.0f);
	sunLight->SetColor({ 1.0f, 0.85f, 0.8f });
	m_LightBuffer->AddLight(sunLight);

	DX12Engine::ModelLoader modelLoader;
	DX12Engine::Mesh cubeMesh = modelLoader.LoadObj(DX12Engine::ResourceManager::GetModelPath("cube.obj"));

	DX12Engine::TextureLoader textureLoader;
	DX12Engine::GPUUploader uploader = m_RenderContext->GetUploader();

	std::vector<DX12Engine::Texture*> textures;
	std::shared_ptr<DX12Engine::Texture> skyboxCube = textureLoader.LoadCubemapDDS(DX12Engine::ResourceManager::GetMaterialPath("skybox/skybox2_cubemap.dds"));
	std::shared_ptr<DX12Engine::Texture> skyboxIrradiance = textureLoader.LoadCubemapDDS(DX12Engine::ResourceManager::GetMaterialPath("skybox/skybox2_irradiance.dds"));
	textures = { skyboxCube.get(), skyboxIrradiance.get() };
	uploader.UploadTextureBatch(textures);

	auto brickTextures = textureLoader.LoadMaterial(DX12Engine::ResourceManager::GetMaterialPath("dark-worn-stone-ue"));
	uploader.UploadTextureBatch(DX12Engine::TextureLoader::GetTextureArray(brickTextures));

	std::shared_ptr<DX12Engine::PBRMaterial> cubeMat = std::make_shared<DX12Engine::PBRMaterial>();
	cubeMat->SetAllTextures(brickTextures);

	std::shared_ptr<DX12Engine::GameObject> cube = std::make_shared<DX12Engine::GameObject>();
	cube->SetMesh(cubeMesh);
	cube->Move({ 0.0f, 1.0f, 0.0f });
	DX12Engine::RenderComponent* cubeRenderComp = cube->CreateComponent<DX12Engine::RenderComponent>();
	cubeRenderComp->SetMaterial(cubeMat);
	m_SceneObjects.Add("Cube", cube);

	std::vector<DX12Engine::RenderComponent*> renderComponents = m_SceneObjects.GetAllComponents<DX12Engine::RenderComponent>();

	DX12Engine::RenderPipelineConfig pipelineConfig;
	DX12Engine::RenderPassConfig geometryConfig;
	geometryConfig.Type = DX12Engine::RenderPassType::Geometry;
	geometryConfig.InputResources[DX12Engine::InputResourceType::SceneObjects] = &renderComponents;
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
	std::vector<std::shared_ptr<DX12Engine::Texture>> lightingExternalTextures{ skyboxCube, skyboxIrradiance };
	DX12Engine::RenderPassConfig lightingConfig;
	lightingConfig.Type = DX12Engine::RenderPassType::Lighting;
	lightingConfig.InputResources[DX12Engine::InputResourceType::Camera] = m_Camera.get();
	lightingConfig.InputResources[DX12Engine::InputResourceType::LightBuffer] = m_LightBuffer.get();
	lightingConfig.InputResources[DX12Engine::InputResourceType::ExternalTextures] = &lightingExternalTextures;
	lightingConfig.InputResources[DX12Engine::InputResourceType::RenderTargets_Geometry] = &gBufferTypes;
	lightingConfig.InputResources[DX12Engine::InputResourceType::RenderTargets_ShadowMap] = &shadowBufferTypes;
	lightingConfig.InputResources[DX12Engine::InputResourceType::RenderTargets_CubeShadowMap] = &cubeShadowBufferTypes;

	std::vector<DX12Engine::RenderTargetType> uiLightingTypes{ DX12Engine::RenderTargetType::Composite };
	DX12Engine::RenderPassConfig uiConfig;
	uiConfig.Type = DX12Engine::RenderPassType::UI;
	uiConfig.InputResources[DX12Engine::InputResourceType::Camera] = m_Camera.get();
	uiConfig.InputResources[DX12Engine::InputResourceType::RenderTargets_Geometry] = &uiGBufferTypes;
	uiConfig.InputResources[DX12Engine::InputResourceType::RenderTargets_Lighting] = &uiLightingTypes;

	pipelineConfig.Passes.push_back(geometryConfig);
	pipelineConfig.Passes.push_back(lightingConfig);
	pipelineConfig.Passes.push_back(uiConfig);
	m_RenderPipeline = m_Renderer->CreateRenderPipeline(pipelineConfig);
}

void ClientApplication::Update(float ts, float elapsed)
{
	m_Camera->ProcessKeyboardInput(ts);

	m_SceneObjects.Update(ts, elapsed);
	m_Renderer->UpdateObjectList(m_SceneObjects.GetAll());

	m_Renderer->ExecutePipeline(m_RenderPipeline);
}

void ClientApplication::HandleMouseMovement(HWND hwnd, LPARAM lParam)
{
	int mouseX = GET_X_LPARAM(lParam);
	int mouseY = GET_Y_LPARAM(lParam);

	if (m_FirstMouse) 
	{
		m_LastMouseX = (float)mouseX;
		m_LastMouseY = (float)mouseY;
		m_FirstMouse = false;
	}

	float deltaX = (float)(mouseX - m_LastMouseX);
	float deltaY = (float)(mouseY - m_LastMouseY);

	m_LastMouseX = (float)mouseX;
	m_LastMouseY = (float)mouseY;

	m_Camera->ProcessMouseInput(deltaX, deltaY);
}
