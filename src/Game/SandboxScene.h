#pragma once
#include "./BrickBuilderScene.h"

namespace BrickBuilder
{
	class SandboxScene : public BrickBuilderScene
	{
	public:
		SandboxScene();
		~SandboxScene();

		virtual void Init() override;
		virtual void Update(float ts, float elapsed) override;

		virtual void SpawnBrick(DirectX::XMVECTOR position) override;
	};
}