#pragma once
#include <memory>
#include <DirectXMath.h>
#include "DX12Engine/Rendering/Buffers/ConstantBuffer.h"

namespace BrickBuilder
{
	struct GridConfig
	{
		DirectX::XMFLOAT3 Origin = { 0.0f, 0.0f, 0.0f };
		float CellSize = 1.0f;
		float LineThickness = 0.02f;
		DirectX::XMFLOAT3 BaseColor = { 0.7f, 0.7f, 0.7f };
		DirectX::XMFLOAT3 LineColor = { 1.0f, 1.0f, 1.0f };
		float Opacity = 0.6f;
	};

	class Grid
	{
	public:
		Grid();
		~Grid();

		DirectX::XMVECTOR AlignToGrid(const DirectX::XMVECTOR& position) const;

		void SetOrigin(const DirectX::XMFLOAT3& origin);
		void SetCellSize(float size);
		void SetLineThickness(float thickness);
		void SetBaseColor(const DirectX::XMFLOAT3& color);
		void SetLineColor(const DirectX::XMFLOAT3& color);
		void SetOpacity(float opacity);

		DX12Engine::ConstantBuffer* GetConstantBuffer() { return m_GridCB.get(); }
		
	private:
		void UpdateConstantBuffer();

		GridConfig m_Config;
		std::unique_ptr<DX12Engine::ConstantBuffer> m_GridCB;
	};
}