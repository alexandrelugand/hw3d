#pragma once

namespace Draw
{
	class Sheet : public DrawableObject
	{
	public:
		Sheet(Graphics& gfx, float scale, const XMFLOAT3& position);
	};
}
