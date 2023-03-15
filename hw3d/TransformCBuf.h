#pragma once

namespace Bind
{
	class TransformCBuf : public Bindable
	{
		struct Transforms
		{
			XMMATRIX modelViewProj;
			XMMATRIX model;
		};

	public:
		TransformCBuf(Graphics& gfx, const Draw::Drawable& parent, UINT slot = 0);
		void Bind(Graphics& gfx) noexcept override;

	private:
		static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
		const Draw::Drawable& parent;
	};
}
