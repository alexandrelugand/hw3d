#pragma once

namespace Draw
{
	class SkinnedBox : public DrawableObject<SkinnedBox>
	{
	public:
		SkinnedBox(Graphics& gfx, float scale, XMFLOAT3 position);
		bool SpawnControlWindow() noexcept;
		void DrawOutline(Graphics& gfx) noexcpt;
		XMMATRIX GetTransform() const noexcept override;

	private:
		void SyncMaterial() noexcpt;

		struct PSMaterialConstant
		{
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[2];
		} materialConstants;

		using MaterialCbuf = Bind::PixelConstantBuffer<PSMaterialConstant>;

		std::vector<std::shared_ptr<Bind::Bindable>> outlineEffect;
		bool outlining = false;
	};
}
