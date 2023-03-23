#pragma once

namespace Draw
{
	class Sheet : public DrawableObject<Sheet>
	{
	public:
		Sheet(Graphics& gfx, float scale, const XMFLOAT3& position);
		bool SpawnControlWindow() noexcept;
	};
}
