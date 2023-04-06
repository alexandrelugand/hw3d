#pragma once

namespace Bind
{
	class SkyboxTransformCBuf : public Bindable
	{
	public:
		SkyboxTransformCBuf(Graphics& gfx, unsigned int slot);
		void Bind(Graphics& gfx) noexcpt override;

	protected:
		struct Transform
		{
			XMMATRIX viewProj;
		};

		void UpdateBindImpl(Graphics& gfx, const Transform& tf) const noexcpt;
		static Transform GetTransform(Graphics& gfx) noexcpt;

	private:
		std::unique_ptr<VertexConstantBuffer<Transform>> pVcbuf{};
	};
}
