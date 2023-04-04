#pragma once

namespace Rgph
{
	class LambertianPass : public RenderQueuePass
	{
	public:
		LambertianPass(Graphics& gfx, std::string name);
		void BindMainCamera(Entities::Camera& camera) noexcept;
		void BindShadowCamera(Entities::Camera& camera) const noexcept;

		void Execute(Graphics& gfx) const noexcpt override;

	private:
		Entities::Camera* pMainCamera = nullptr;
		std::shared_ptr<Bind::ShadowSampler> pShadowSampler;
		std::shared_ptr<Bind::ShadowCameraCBuf> pShadowCBuf;
	};
}
