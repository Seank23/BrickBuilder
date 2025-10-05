#pragma once
#include "DX12Engine/Entity/Scene.h"

namespace BrickBuilder
{
	class SandboxScene : public DX12Engine::Scene
	{
	public:
		SandboxScene() = default;
		~SandboxScene() = default;

		virtual void Init();
		virtual void Update(float ts, float elapsed);

	private:
	};
}