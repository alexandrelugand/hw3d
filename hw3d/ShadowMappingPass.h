#pragma once
#include "Forwards.h"

namespace Rgph
{
	class ShadowMappingPass : public RenderQueuePass
	{
	public:
		ShadowMappingPass(Graphics& gfx, std::string name);

		void BindShadowCamera(Entities::Camera& camera);
		void Execute(Graphics& gfx) const noexcpt override;

		void DumpShadowMap(Graphics& gfx, const std::string& path) const;

	private:
		void SetDepthBuffer(std::shared_ptr<Bind::DepthStencil> ds) const;
		static constexpr UINT size = 1000;

		Entities::Camera* pShadowCamera = nullptr;
		std::shared_ptr<Bind::DepthCubeTexture> pDepthCube;
		XMFLOAT4X4 projection;
		std::vector<XMFLOAT3> cameraDirections{6};
		std::vector<XMFLOAT3> cameraUps{6};
	};
}
