#include "Grid.h"
#include "DX12Engine/Resources/ResourceManager.h"

namespace BrickBuilder
{
	Grid::Grid()
	{
		m_GridCB = DX12Engine::ResourceManager::GetInstance().CreateConstantBuffer(sizeof(GridConfig));
		UpdateConstantBuffer();
	}

	Grid::~Grid()
	{
	}

	DirectX::XMVECTOR Grid::AlignToGrid(const DirectX::XMVECTOR& position) const
	{
		DirectX::XMVECTOR newPosition = DirectX::XMVectorSubtract(position, DirectX::XMLoadFloat3(&m_Config.Origin));
		DirectX::XMVECTOR cellOffset = DirectX::XMVectorMod(newPosition, DirectX::XMVectorReplicate(m_Config.CellSize));
		newPosition = DirectX::XMVectorSubtract(newPosition, cellOffset);
		newPosition = { 
			DirectX::XMVectorGetX(position) >= 0.0f ? DirectX::XMVectorGetX(newPosition) + (m_Config.CellSize / 2) : DirectX::XMVectorGetX(newPosition) - (m_Config.CellSize / 2),
			DirectX::XMVectorGetY(newPosition),
			DirectX::XMVectorGetZ(position) >= 0.0f ? DirectX::XMVectorGetZ(newPosition) + (m_Config.CellSize / 2) : DirectX::XMVectorGetZ(newPosition) - (m_Config.CellSize / 2) 
		};
		return newPosition;
	}

	void Grid::SetOrigin(const DirectX::XMFLOAT3& origin)
	{
		m_Config.Origin = origin;
		UpdateConstantBuffer();
	}

	void Grid::SetCellSize(float size)
	{
		m_Config.CellSize = size;
		UpdateConstantBuffer();
	}

	void Grid::SetLineThickness(float thickness)
	{
		m_Config.LineThickness = thickness;
		UpdateConstantBuffer();
	}

	void Grid::SetBaseColor(const DirectX::XMFLOAT3& color)
	{
		m_Config.BaseColor = color;
		UpdateConstantBuffer();
	}

	void Grid::SetLineColor(const DirectX::XMFLOAT3& color)
	{
		m_Config.LineColor = color;
		UpdateConstantBuffer();
	}

	void Grid::SetOpacity(float opacity)
	{
		m_Config.Opacity = opacity;
		UpdateConstantBuffer();
	}

	void Grid::UpdateConstantBuffer()
	{
		m_GridCB->Update(&m_Config, sizeof(GridConfig));
	}
}