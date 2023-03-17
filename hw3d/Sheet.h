#pragma once

namespace Draw
{
	class Sheet : public DrawableObject<Sheet>
	{
	public:
		Sheet(Graphics& gfx, float size);
		bool SpawnControlWindow() noexcept;

	private:
		void SyncConstPS() noexcpt;

		struct BumpMappingConstant
		{
			float specularIntensity = 0.1f;
			float specularPower = 20.0f;
			BOOL normalMappingEnabled = TRUE;
			float padding[1];
		} pmc;

		using ConstPS = Bind::PixelConstantBuffer<BumpMappingConstant>;
	};
}
