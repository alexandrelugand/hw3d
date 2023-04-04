#include "stdafx.h"
#include "ColorCBuf.h"

namespace Bind
{
	std::unique_ptr<PixelConstantBuffer<ColorCBuf::ColorBuf>> ColorCBuf::pPcbuf;

	ColorCBuf::ColorCBuf(Graphics& gfx, const Draw::ColorDrawable& parent, unsigned int slot)
		: parent(parent)
	{
		if (!pPcbuf)
			pPcbuf = std::make_unique<PixelConstantBuffer<ColorBuf>>(gfx, slot);
	}

	void ColorCBuf::Bind(Graphics& gfx) noexcpt
	{
		cbData.color = parent.GetColor();
		pPcbuf->Update(gfx, cbData);
		pPcbuf->Bind(gfx);
	}
}
