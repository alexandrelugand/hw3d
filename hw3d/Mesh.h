#pragma once

namespace Entities
{
	class Mesh : public Draw::DrawableBase<Mesh>
	{
	public:
		Mesh(Graphics& gfx, std::vector<std::unique_ptr<Bind::Bindable>> bindPtrs);

		void Draw(Graphics& gfx, FXMMATRIX accumulatedTransform) const noexcpt;
		XMMATRIX GetTransform() const noexcept override;

	private:
		mutable XMFLOAT4X4 transform;
	};
}
