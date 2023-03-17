#pragma once

namespace Draw
{
	class Asset : public DrawableObject<Asset>
	{
	public:
		Asset(Graphics& gfx, XMFLOAT3 material, float scale);
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
