#pragma once

namespace Draw
{
	class Pyramid : public DrawableObject<Pyramid>
	{
	public:
		Pyramid(Graphics& gfx, XMFLOAT3 material);
		bool SpawnControlWindow() noexcept;

	private:
		void SyncMaterial() noexcpt;

		struct PSMaterialConstant
		{
			XMFLOAT3 color;
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[3];
		} materialConstants;

		using MaterialCbuf = Bind::PixelConstantBuffer<PSMaterialConstant>;
	};
}
