#pragma once

namespace Bind
{
	class ShadowCameraCBuf : public Bindable
	{
	protected:
		struct Transform
		{
			XMMATRIX ViewProj;
		};

	public:
		ShadowCameraCBuf(Graphics& gfx, UINT slot = 1u);

		void Bind(Graphics& gfx) noexcpt override;
		void Update(Graphics& gfx) const;
		void SetCamera(const Entities::Camera* pCamera) noexcept;

	private:
		std::unique_ptr<VertexConstantBuffer<Transform>> pVcbuf;
		const Entities::Camera* pCamera = nullptr;
	};
}
