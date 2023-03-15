#pragma once

namespace Draw
{
	class ColorDrawable : public DrawableBase<ColorDrawable>
	{
	public:
		ColorDrawable(XMFLOAT3 color)
			: color(color)
		{
		}

		const XMFLOAT3& GetColor() const noexcept
		{
			return color;
		}

		void SetColor(const XMFLOAT3& col) noexcept
		{
			color = col;
		}

	private:
		XMFLOAT3 color;
	};
}
