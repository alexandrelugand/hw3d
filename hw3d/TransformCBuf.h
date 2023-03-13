#pragma once

class TransformCBuf : public Bindable
{
	struct Transforms
	{
		XMMATRIX modelViewProj;
		XMMATRIX model;
	};

public:
	TransformCBuf(Graphics& gfx, const Drawable& parent, UINT slot = 0);
	void Bind(Graphics& gfx) noexcept override;

private:
	static std::unique_ptr<VertexConstantBuffer<Transforms>> pVcbuf;
	const Drawable& parent;
};
