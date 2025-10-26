#pragma once
#include "DX12Engine/Entity/Scene.h"
#include "DX12Engine/Resources/Mesh.h"
#include "DX12Engine/Resources/Materials/PBRMaterial.h"
#include "Grid.h"

namespace BrickBuilder
{
	class GameContext;

	class BrickBuilderScene : public DX12Engine::Scene
	{
	public:
		BrickBuilderScene(GameContext& context)
			: m_GameContext(context)
		{
			m_Grid = std::make_unique<Grid>();
		}
		~BrickBuilderScene() = default;

		Grid* GetGrid() { return m_Grid.get(); }

		virtual void SpawnBrick(DirectX::XMVECTOR position) = 0;

	protected:
		std::unordered_map<std::string, std::shared_ptr<DX12Engine::Mesh>> m_BrickMeshes;
		std::unordered_map<std::string, std::shared_ptr<DX12Engine::PBRMaterial>> m_BrickMaterials;
		int m_BrickCount = 0;

		GameContext& m_GameContext;
		std::unique_ptr<Grid> m_Grid;
	};
}