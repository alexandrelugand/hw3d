#include "stdafx.h"
#include "SkyboxPass.h"


namespace Rgph
{
	SkyboxPass::SkyboxPass(Graphics& gfx, std::string name)
		: BindingPass(std::move(name))
	{
		RegisterSink(DirectBufferSink<Bind::RenderTarget>::Make("renderTarget", renderTarget));
		RegisterSink(DirectBufferSink<Bind::DepthStencil>::Make("depthStencil", depthStencil));
		//AddBind(std::make_shared<Bind::CubeTexture>(gfx, "Images\\SpaceBox", Shaders::Texture::Skybox));
		AddBind(std::make_shared<Bind::CubeTexture>(gfx, "Images\\Skybox", Shaders::Texture::Skybox));
		AddBind(Bind::Stencil::Resolve(gfx, Bind::Stencil::Mode::DepthFirst));
		AddBind(Bind::Sampler::Resolve(gfx, Bind::Sampler::Type::Bilinear));
		AddBind(Bind::Rasterizer::Resolve(gfx, CullMode::Front));
		AddBind(std::make_shared<Bind::SkyboxTransformCBuf>(gfx, Shaders::CBuf::Skybox));
		AddBind(Bind::PixelShader::Resolve(gfx, "Skybox_PS.cso"));
		AddBind(Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		{
			// geometry-related
			auto pvs = Bind::VertexShader::Resolve(gfx, "Skybox_VS.cso");
			{
				// cube
				auto model = Geometry::Cube::Make();
				const auto tag = "$cube_map";
				pCubeVertices = Bind::VertexBuffer::Resolve(gfx, tag, std::move(model.vertices));
				pCubeIndices = Bind::IndexBuffer::Resolve(gfx, tag, std::move(model.indices));
				cubeCount = static_cast<unsigned int>(model.indices.size());
				// layout is shared between cube and sphere; use cube data to generate
				AddBind(Bind::InputLayout::Resolve(gfx, model.vertices.GetLayout(), *pvs));
			}
			{
				// sphere
				auto model = Geometry::Sphere::Make(12, 24);
				const auto tag = "$sphere_map";
				pSphereVertices = Bind::VertexBuffer::Resolve(gfx, tag, std::move(model.vertices));
				pSphereIndices = Bind::IndexBuffer::Resolve(gfx, tag, std::move(model.indices));
				sphereCount = static_cast<unsigned int>(model.indices.size());
			}
			AddBind(std::move(pvs));
		}
		RegisterSource(DirectBufferSource<Bind::RenderTarget>::Make("renderTarget", renderTarget));
		RegisterSource(DirectBufferSource<Bind::DepthStencil>::Make("depthStencil", depthStencil));
	}

	void SkyboxPass::BindMainCamera(Entities::Camera& camera) noexcept
	{
		pMainCamera = &camera;
	}

	void SkyboxPass::Execute(Graphics& gfx) const noexcpt
	{
		assert(pMainCamera);
		unsigned int indexCount;
		pMainCamera->BindToGraphics(gfx);
		if (useSphere)
		{
			pSphereVertices->Bind(gfx);
			pSphereIndices->Bind(gfx);
			indexCount = sphereCount;
		}
		else
		{
			pCubeVertices->Bind(gfx);
			pCubeIndices->Bind(gfx);
			indexCount = cubeCount;
		}
		BindAll(gfx);
		gfx.DrawIndexed(indexCount);
	}

	void SkyboxPass::RenderWindow()
	{
		if (ImGui::Begin("Skybox"))
			ImGui::Checkbox("Use sphere", &useSphere);
		ImGui::End();
	}
}
