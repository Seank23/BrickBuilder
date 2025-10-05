#include "SandboxScene.h"

#include "DX12Engine/IO/ModelLoader.h"
#include "DX12Engine/Resources/Mesh.h"
#include "DX12Engine/Entity/RenderComponent.h"
#include "DX12Engine/IO/TextureLoader.h"
#include "DX12Engine/Resources/Texture.h"
#include "DX12Engine/Rendering/GPUUploader.h"
#include "DX12Engine/Resources/Materials/PBRMaterial.h"
#include <DX12Engine/Resources/ResourceManager.h>

namespace BrickBuilder
{
	void SandboxScene::Init()
	{
		m_Camera = std::make_unique<DX12Engine::Camera>(16.0f / 9.0f, 0.1f, 100.0f);
		m_Camera->SetPosition({ -5.0f, 1.0f, 3.0f });
		m_Camera->SetRotation(0.0f, -30.0f);

		m_LightBuffer = std::make_unique<DX12Engine::LightBuffer>();
		std::shared_ptr<DX12Engine::Light> sunLight = std::make_shared<DX12Engine::Light>();
		sunLight->SetType((int)DX12Engine::LightType::Directional);
		sunLight->SetDirection({ 0.45f, -0.577f, 0.577f });
		sunLight->SetIntensity(5.0f);
		sunLight->SetColor({ 1.0f, 0.85f, 0.8f });
		m_LightBuffer->AddLight(sunLight);

		DX12Engine::TextureLoader textureLoader;
		m_SkyboxCubemap = textureLoader.LoadCubemapDDS(DX12Engine::ResourceManager::GetMaterialPath("skybox/skybox2_cubemap.dds"));
		m_SkyboxIrradiance = textureLoader.LoadCubemapDDS(DX12Engine::ResourceManager::GetMaterialPath("skybox/skybox2_irradiance.dds"));

		auto brickTextures = textureLoader.LoadMaterial(DX12Engine::ResourceManager::GetMaterialPath("dark-worn-stone-ue"));
		std::shared_ptr<DX12Engine::PBRMaterial> cubeMat = std::make_shared<DX12Engine::PBRMaterial>();
		cubeMat->SetAllTextures(brickTextures);

		DX12Engine::ModelLoader modelLoader;
		DX12Engine::Mesh cubeMesh = modelLoader.LoadObj(DX12Engine::ResourceManager::GetModelPath("cube.obj"));

		std::shared_ptr<DX12Engine::GameObject> cube = std::make_shared<DX12Engine::GameObject>();
		cube->SetMesh(cubeMesh);
		cube->Move({ 0.0f, 1.0f, 0.0f });
		DX12Engine::RenderComponent* cubeRenderComp = cube->CreateComponent<DX12Engine::RenderComponent>();
		cubeRenderComp->SetMaterial(cubeMat);
		m_SceneObjects.Add("Cube", cube);
	}

	void SandboxScene::Update(float ts, float elapsed)
	{
		m_Camera->Update(ts);
		m_SceneObjects.Update(ts, elapsed);
	}
}