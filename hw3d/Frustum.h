#pragma once
#include "Drawable.h"

namespace Draw
{
	class Frustum : public Drawable
	{
	public:
		Frustum(Graphics& gfx, float width, float height, float nearZ, float farZ);

		void SetVertices(Graphics& gfx, float width, float height, float nearZ, float farZ);
		void SetPos(XMFLOAT3 pos) noexcept;
		void SetRot(XMFLOAT3 rot) noexcept;
		XMMATRIX GetTransform() const noexcept override;

	private:
		XMFLOAT3 pos{0.0f, 0.0f, 0.0f};
		XMFLOAT3 rot{0.0f, 0.0f, 0.0f};
	};
}
