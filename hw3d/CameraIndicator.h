#pragma once

namespace Draw
{
	class CameraIndicator : public Drawable
	{
	public:
		CameraIndicator(Graphics& gfx);

		void SetPos(XMFLOAT3 pos) noexcept;
		void SetRot(XMFLOAT3 rot) noexcept;
		XMMATRIX GetTransform() const noexcept override;

	private:
		XMFLOAT3 pos{0.0f, 0.0f, 0.0f};
		XMFLOAT3 rot{0.0f, 0.0f, 0.0f};
	};
}
