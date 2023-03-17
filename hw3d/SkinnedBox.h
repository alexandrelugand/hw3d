#pragma once

namespace Draw
{
	class SkinnedBox : public DrawableObject<SkinnedBox>
	{
	public:
		SkinnedBox(Graphics& gfx);
		bool SpawnControlWindow() noexcept;

	private:
		void SyncMaterial() noexcpt;

		struct PSMaterialConstant
		{
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[2];
		} materialConstants;

		using MaterialCbuf = Bind::PixelConstantBuffer<PSMaterialConstant>;
	};
}
