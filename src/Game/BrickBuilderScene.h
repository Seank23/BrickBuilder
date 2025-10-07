#pragma once
#include "DX12Engine/Entity/Scene.h"
#include "DX12Engine/Resources/Mesh.h"
#include "DX12Engine/Resources/Materials/PBRMaterial.h"

namespace BrickBuilder
{
	class BrickBuilderScene : public DX12Engine::Scene
	{
	public:
		BrickBuilderScene() = default;
		~BrickBuilderScene() = default;

		virtual void Init() = 0;
		virtual void Update(float ts, float elapsed) = 0;

		virtual void SpawnBrick(DirectX::XMVECTOR position) = 0;

	protected:
		std::unordered_map<std::string, std::shared_ptr<DX12Engine::Mesh>> m_BrickMeshes;
		std::unordered_map<std::string, std::shared_ptr<DX12Engine::PBRMaterial>> m_BrickMaterials;
		int m_BrickCount = 0;
	};
}