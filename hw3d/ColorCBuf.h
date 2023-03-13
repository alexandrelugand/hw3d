#pragma once

class ColorCBuf : public Bindable
{
	struct ColorBuf
	{
		XMFLOAT3 color;
		float padding;
	};

public:
	ColorCBuf(Graphics& gfx, const ColorDrawable& parent, UINT slot = 0);
	void Bind(Graphics& gfx) noexcpt override;

private:
	ColorBuf cbData{};
	const ColorDrawable& parent;
	static std::unique_ptr<PixelConstantBuffer<ColorBuf>> pPcbuf;
};
