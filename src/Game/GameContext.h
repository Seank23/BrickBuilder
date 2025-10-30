#pragma once
#include <memory>
#include "./BrickBuilderScene.h"
#include <DirectXMath.h>

namespace BrickBuilder
{
	class InputHandler;
	class Camera;
	class LightBuffer;

	class GameContext
	{
	public:
		GameContext();
		~GameContext();

		void Init(DirectX::XMFLOAT2 windowSize);
		void Update(float ts, float elapsed);

		void HandleWindowEvent(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		InputHandler* GetInputHandler() { return m_InputHandler.get(); }
		BrickBuilderScene* GetScene() { return m_Scene.get(); }

		void LoadMesh(const std::string& meshName, std::shared_ptr<DX12Engine::Mesh> mesh);
		void LoadMaterial(const std::string& materialName, std::shared_ptr<DX12Engine::PBRMaterial> material);

		std::shared_ptr<DX12Engine::Mesh> GetCurrentMesh() { return m_MeshIterator->second; }
		std::shared_ptr<DX12Engine::Mesh> GetMesh(std::string name) { return m_BrickMeshes[name]; }
		std::shared_ptr<DX12Engine::PBRMaterial> GetCurrentMaterial() { return m_MaterialIterator->second; }
		std::shared_ptr<DX12Engine::PBRMaterial> GetMaterial(std::string name) { return m_BrickMaterials[name]; }

		void CycleMaterial(bool forward);

	private:
		void OnResize(DirectX::XMFLOAT2 newSize);

		std::unique_ptr<InputHandler> m_InputHandler;
		std::unique_ptr<BrickBuilderScene> m_Scene;

		std::unordered_map<std::string, std::shared_ptr<DX12Engine::Mesh>> m_BrickMeshes;
		std::unordered_map<std::string, std::shared_ptr<DX12Engine::Mesh>>::iterator m_MeshIterator;
		std::unordered_map<std::string, std::shared_ptr<DX12Engine::PBRMaterial>> m_BrickMaterials;
		std::unordered_map<std::string, std::shared_ptr<DX12Engine::PBRMaterial>>::iterator m_MaterialIterator;
	};
}