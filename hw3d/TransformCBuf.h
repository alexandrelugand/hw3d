#pragma once

namespace Bind
{
	class TransformCBuf : public Bindable
	{
	public:
		TransformCBuf(Graphics& gfx, UINT slot = 0u);
		void Bind(Graphics& gfx) noexcept override;
		void InitializeParentReferences(const Draw::Drawable& parent) noexcept override;

	protected:
		struct Transforms
		{
			XMMATRIX modelView;
			XMMATRIX modelViewProj;
		};

		virtual void UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcept;
		Transforms GetTransforms(Graphics& gfx) noexcept;

	private:
		static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
		const Draw::Drawable* pParent = nullptr;
	};
}
