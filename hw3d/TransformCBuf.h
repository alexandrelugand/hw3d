#pragma once

namespace Bind
{
	class TransformCBuf : public CloningBindable
	{
	public:
		TransformCBuf(Graphics& gfx, unsigned int slot);
		void Bind(Graphics& gfx) noexcpt override;
		void InitializeParentReferences(const Draw::Drawable& parent) noexcept override;
		std::unique_ptr<CloningBindable> Clone() const noexcept override;

	protected:
		struct Transforms
		{
			XMMATRIX model;
			XMMATRIX modelView;
			XMMATRIX modelViewProj;
		};

		virtual void UpdateBindImpl(Graphics& gfx, const Transforms& tf) noexcpt;
		Transforms GetTransforms(Graphics& gfx) noexcpt;

	private:
		static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
		const Draw::Drawable* pParent = nullptr;
	};
}
