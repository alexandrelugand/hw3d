#pragma once

namespace Draw
{
	class SkinnedBox : public DrawableObject
	{
	public:
		SkinnedBox(Graphics& gfx, float scale, XMFLOAT3 position = {0.0f, 0.0f, 0.0f});
	};
}
