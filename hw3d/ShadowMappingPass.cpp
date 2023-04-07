#include "stdafx.h"
#include "ShadowMappingPass.h"


namespace Rgph
{
	ShadowMappingPass::ShadowMappingPass(Graphics& gfx, std::string name)
		: RenderQueuePass(std::move(name))
	{
		pDepthCube = std::make_shared<Bind::DepthCubeTexture>(gfx, size, Shaders::Texture::ShadowMap);
		AddBind(Bind::VertexShader::Resolve(gfx, "Shadow_VS.cso"));
		AddBind(Bind::NullPixelShader::Resolve(gfx));
		AddBind(Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::Off));
		AddBind(Bind::Blender::Resolve(gfx, false));
		AddBind(std::make_shared<Bind::Viewport>(gfx, static_cast<float>(size), static_cast<float>(size)));
		AddBind(std::make_shared<Bind::ShadowRasterizer>(gfx, 50, 2.0f, 0.1f));
		RegisterSource(DirectBindableSource<Bind::DepthCubeTexture>::Make("map", pDepthCube));

		XMStoreFloat4x4(
			&projection,
			XMMatrixPerspectiveFovLH(PI / 2.0f, 1.0f, 0.5f, 100.0f)
		);
		// +x
		XMStoreFloat3(&cameraDirections[0], XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f));
		XMStoreFloat3(&cameraUps[0], XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		// -x
		XMStoreFloat3(&cameraDirections[1], XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f));
		XMStoreFloat3(&cameraUps[1], XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		// +y
		XMStoreFloat3(&cameraDirections[2], XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		XMStoreFloat3(&cameraUps[2], XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f));
		// -y
		XMStoreFloat3(&cameraDirections[3], XMVectorSet(0.0f, -1.0f, 0.0f, 0.0f));
		XMStoreFloat3(&cameraUps[3], XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
		// +z
		XMStoreFloat3(&cameraDirections[4], XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f));
		XMStoreFloat3(&cameraUps[4], XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
		// -z
		XMStoreFloat3(&cameraDirections[5], XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f));
		XMStoreFloat3(&cameraUps[5], XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

		// set the depth buffer to *something* so that the rg validation doesn't scream
		SetDepthBuffer(pDepthCube->GetDepthBuffer(0));
	}

	void ShadowMappingPass::BindShadowCamera(Entities::Camera& camera)
	{
		pShadowCamera = &camera;
	}

	void ShadowMappingPass::Execute(Graphics& gfx) const noexcpt
	{
		assert(pShadowCamera);
		const auto shadowCamPos = pShadowCamera->GetPos();
		const auto pos = XMLoadFloat3(&shadowCamPos);

		gfx.SetProjection(XMLoadFloat4x4(&projection));
		for (size_t i = 0; i < 6; i++)
		{
			auto d = pDepthCube->GetDepthBuffer(i);
			d->Clear(gfx);
			SetDepthBuffer(std::move(d));
			const auto lookAt = pos + XMLoadFloat3(&cameraDirections[i]);
			gfx.SetCamera(XMMatrixLookAtLH(pos, lookAt, XMLoadFloat3(&cameraUps[i])));
			RenderQueuePass::Execute(gfx);
		}
	}

	void ShadowMappingPass::DumpShadowMap(Graphics& gfx, const std::string& path) const
	{
		for (size_t i = 0; i < 6; i++)
		{
			auto d = pDepthCube->GetDepthBuffer(i);
			d->ToSurface(gfx).Save(path + std::to_string(i) + ".png");
		}
	}

	void ShadowMappingPass::SetDepthBuffer(std::shared_ptr<Bind::DepthStencil> ds) const
	{
		const_cast<ShadowMappingPass*>(this)->depthStencil = std::move(ds);
	}
}
