#pragma once

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
		Entities::Camera* pShadowCamera = nullptr;
	};
}
