#pragma once

namespace Entities
{
	class Mesh : public Draw::DrawableObject
	{
	public:
		Mesh(Graphics& gfx, const Material& mat, const aiMesh& mesh, float scale = 1.0f);
		XMMATRIX GetTransform() const noexcept override;
		void Submit(FXMMATRIX accumulatedTransform) const noexcpt;

	private:
		mutable XMFLOAT4X4 transform{};
	};
}
