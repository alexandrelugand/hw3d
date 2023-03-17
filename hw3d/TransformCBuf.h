#pragma once

namespace Bind
{
	class TransformCBuf : public Bindable
	{
	public:
		TransformCBuf(Graphics& gfx, const Draw::Drawable& parent, UINT slot = 0u);
		void Bind(Graphics& gfx) noexcept override;

	protected:
		struct Transforms
		{
			XMMATRIX modelViewProj;
			XMMATRIX model;
		};

		void UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept;
		Transforms GetTransforms(Graphics& gfx) noexcept;

	private:
		static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
		const Draw::Drawable& parent;
	};
}
