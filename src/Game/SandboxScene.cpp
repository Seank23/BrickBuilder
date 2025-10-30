#include "SandboxScene.h"

#include "DX12Engine/IO/ModelLoader.h"
#include "DX12Engine/Entity/RenderComponent.h"
#include "DX12Engine/IO/TextureLoader.h"
#include "DX12Engine/Resources/Texture.h"
#include "DX12Engine/Rendering/GPUUploader.h"
#include <DX12Engine/Resources/ResourceManager.h>
#include "GameContext.h"

namespace BrickBuilder
{
	SandboxScene::SandboxScene(GameContext& context)
		: BrickBuilderScene(context)
	{
	}

	SandboxScene::~SandboxScene()
	{
	}

	void SandboxScene::Init()
	{
		m_Camera = std::make_unique<DX12Engine::Camera>(16.0f / 9.0f, 0.1f, 100.0f);
		m_Camera->SetPosition({ -10.0f, 2.0f, 6.0f });
		m_Camera->SetRotation(0.0f, -30.0f);

		m_Grid->SetCellSize(2.0f);

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

		std::shared_ptr<DX12Engine::PBRMaterial> brickMat = std::make_shared<DX12Engine::PBRMaterial>();
		brickMat->SetAllTextures(textureLoader.LoadMaterial(DX12Engine::ResourceManager::GetMaterialPath("dark-worn-stone-ue")));
		m_GameContext.LoadMaterial("Brick", brickMat);

		std::shared_ptr<DX12Engine::PBRMaterial> goldMat = std::make_shared<DX12Engine::PBRMaterial>();
		goldMat->SetAllTextures(textureLoader.LoadMaterial(DX12Engine::ResourceManager::GetMaterialPath("hammered-gold-ue")));
		m_GameContext.LoadMaterial("Gold", goldMat);

		std::shared_ptr<DX12Engine::PBRMaterial> concreteMat = std::make_shared<DX12Engine::PBRMaterial>();
		concreteMat->SetAllTextures(textureLoader.LoadMaterial(DX12Engine::ResourceManager::GetMaterialPath("clean-concrete-ue")));
		m_GameContext.LoadMaterial("Concrete", concreteMat);

		std::shared_ptr<DX12Engine::PBRMaterial> metalMat = std::make_shared<DX12Engine::PBRMaterial>();
		metalMat->SetAllTextures(textureLoader.LoadMaterial(DX12Engine::ResourceManager::GetMaterialPath("worn-shiny-metal-ue")));
		m_GameContext.LoadMaterial("Metal", metalMat);

		DX12Engine::ModelLoader modelLoader;
		DX12Engine::Mesh cubeMesh = modelLoader.LoadObj(DX12Engine::ResourceManager::GetModelPath("cube.obj"));
		m_GameContext.LoadMesh("Cube", std::make_shared<DX12Engine::Mesh>(cubeMesh));

		SpawnBrick({ 0.0f, 0.0f, 0.0f });
	}

	void SandboxScene::Update(float ts, float elapsed)
	{
		m_Camera->Update(ts);
		m_SceneObjects.Update(ts, elapsed);
	}

	void SandboxScene::SpawnBrick(DirectX::XMVECTOR position)
	{
		std::unique_ptr<DX12Engine::GameObject> cube = std::make_unique<DX12Engine::GameObject>();
		cube->SetMesh(m_GameContext.GetMesh("Cube"));
		cube->Move(m_Grid->AlignToGrid(position));
		cube->Move({ 0.0f, 1.0f, 0.0f });
		DX12Engine::RenderComponent* cubeRenderComp = cube->CreateComponent<DX12Engine::RenderComponent>();
		cubeRenderComp->SetMaterial(m_GameContext.GetCurrentMaterial());
		m_SceneObjects.Add("Cube_" + std::to_string(m_BrickCount), std::move(cube));
		m_BrickCount++;
	}
}