#pragma once

namespace Draw
{
	class SolidSphere : public ColorDrawable
	{
	public:
		SolidSphere(Graphics& gfx, float radius, XMFLOAT3 color);
		void SetPos(XMFLOAT3 pos) noexcept;
		XMMATRIX GetTransform() const noexcept override;

	protected:
		XMFLOAT3 position = {1.0f, 1.0f, 1.0f};
	};
}
