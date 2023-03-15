#pragma once

namespace Draw
{
	class AssTest : public TestObject<AssTest>
	{
	public:
		AssTest(Graphics& gfx, std::mt19937& rng,
		        std::uniform_real_distribution<float>& adist,
		        std::uniform_real_distribution<float>& ddist,
		        std::uniform_real_distribution<float>& odist,
		        std::uniform_real_distribution<float>& rdist,
		        std::uniform_real_distribution<float>& bdist,
		        XMFLOAT3 material,
		        float scale);

	private:
		void SyncMaterial(Graphics& gfx) noexcpt;

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
