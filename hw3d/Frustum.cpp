#include "stdafx.h"
#include "Frustum.h"

namespace Draw
{
	Frustum::Frustum(Graphics& gfx, float width, float height, float nearZ, float farZ)
	{
		std::vector<unsigned short> indices;
		{
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(1);
			indices.push_back(2);
			indices.push_back(2);
			indices.push_back(3);
			indices.push_back(3);
			indices.push_back(0);
			indices.push_back(4);
			indices.push_back(5);
			indices.push_back(5);
			indices.push_back(6);
			indices.push_back(6);
			indices.push_back(7);
			indices.push_back(7);
			indices.push_back(4);
			indices.push_back(0);
			indices.push_back(4);
			indices.push_back(1);
			indices.push_back(5);
			indices.push_back(2);
			indices.push_back(6);
			indices.push_back(3);
			indices.push_back(7);
		}

		SetVertices(gfx, width, height, nearZ, farZ);
		pIndices = Bind::IndexBuffer::Resolve(gfx, "$frustrum", indices);
		pTopology = Bind::Topology::Resolve(gfx, D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

		Rgph::Technique line{Chan::main};
		{
			Rgph::Step unoccluded("lambertian");
			{
				auto pvs = Bind::VertexShader::Resolve(gfx, "Solid_VS.cso");
				unoccluded.AddBindable(Bind::InputLayout::Resolve(gfx, pVertices->GetLayout(), *pvs));
				unoccluded.AddBindable(std::move(pvs));

				unoccluded.AddBindable(Bind::PixelShader::Resolve(gfx, "Solid_PS.cso"));

				struct PSColorConstant
				{
					XMFLOAT3 color{0.6f, 0.2f, 0.2f};
					float padding{};
				} colorConst;
				unoccluded.AddBindable(Bind::PixelConstantBuffer<PSColorConstant>::Resolve(gfx, colorConst, 1u));

				unoccluded.AddBindable(std::make_shared<Bind::TransformCBuf>(gfx));
				unoccluded.AddBindable(Bind::Rasterizer::Resolve(gfx, CullMode::Back));
				line.AddStep(std::move(unoccluded));
			}

			Rgph::Step occluded("wireframe");
			{
				auto pvs = Bind::VertexShader::Resolve(gfx, "Solid_VS.cso");
				occluded.AddBindable(Bind::InputLayout::Resolve(gfx, pVertices->GetLayout(), *pvs));
				occluded.AddBindable(std::move(pvs));

				occluded.AddBindable(Bind::PixelShader::Resolve(gfx, "Solid_PS.cso"));

				struct PSColorConstant2
				{
					XMFLOAT3 color{0.25f, 0.08f, 0.08f};
					float padding{};
				} colorConst;
				occluded.AddBindable(Bind::PixelConstantBuffer<PSColorConstant2>::Resolve(gfx, colorConst, 1u));

				occluded.AddBindable(std::make_shared<Bind::TransformCBuf>(gfx));
				occluded.AddBindable(Bind::Rasterizer::Resolve(gfx, CullMode::Back));
				line.AddStep(std::move(occluded));
			}
			AddTechnique(std::move(line));
		}
	}

	void Frustum::SetVertices(Graphics& gfx, float width, float height, float nearZ, float farZ)
	{
		Dvtx::VertexLayout layout;
		layout.Append(ElementType::Position3D);
		Dvtx::VertexBufferDescriptor vbd(std::move(layout));
		{
			const float zRatio = farZ / nearZ;
			const float nearX = width / 2.0f;
			const float nearY = height / 2.0f;
			const float farX = nearX * zRatio;
			const float farY = nearY * zRatio;
			vbd.EmplaceBack(XMFLOAT3{-nearX, nearY, nearZ});
			vbd.EmplaceBack(XMFLOAT3{nearX, nearY, nearZ});
			vbd.EmplaceBack(XMFLOAT3{nearX, -nearY, nearZ});
			vbd.EmplaceBack(XMFLOAT3{-nearX, -nearY, nearZ});
			vbd.EmplaceBack(XMFLOAT3{-farX, farY, farZ});
			vbd.EmplaceBack(XMFLOAT3{farX, farY, farZ});
			vbd.EmplaceBack(XMFLOAT3{farX, -farY, farZ});
			vbd.EmplaceBack(XMFLOAT3{-farX, -farY, farZ});
		}
		pVertices = std::make_shared<Bind::VertexBuffer>(gfx, vbd);
	}

	void Frustum::SetPos(XMFLOAT3 pos) noexcept
	{
		this->pos = pos;
	}

	void Frustum::SetRot(XMFLOAT3 rot) noexcept
	{
		this->rot = rot;
	}

	XMMATRIX Frustum::GetTransform() const noexcept
	{
		return XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rot)) *
			XMMatrixTranslationFromVector(XMLoadFloat3(&pos));
	}
}
