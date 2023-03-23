#pragma once

namespace Entities
{
	class Mesh : public Draw::DrawableObject<Mesh>
	{
	public:
		Mesh(Graphics& gfx);
		XMMATRIX GetTransform() const noexcept override;
		void Submit(FrameCommander& frame, FXMMATRIX accumulatedTransform) const noexcpt;

	private:
		mutable XMFLOAT4X4 transform{};
	};
}
