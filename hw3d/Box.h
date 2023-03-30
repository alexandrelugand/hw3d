#pragma once

namespace Draw
{
	class Box : public DrawableObject
	{
	public:
		Box(Graphics& gfx, XMFLOAT3 material, float scale = 1.0f, const XMFLOAT3& position = {0.0f, 0.0f, 0.0f});
	};
}
