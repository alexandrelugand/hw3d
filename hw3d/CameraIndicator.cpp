#include "stdafx.h"
#include "CameraIndicator.h"

namespace Draw
{
	CameraIndicator::CameraIndicator(Graphics& gfx)
	{
		const auto tag = "$cam";
		Dvtx::VertexLayout layout;
		layout.Append(ElementType::Position3D);
		Dvtx::VertexBufferDescriptor vbd(std::move(layout));
		{
			const float x = 4.0f / 3.0f * 0.75f;
			const float y = 1.0f * 0.75f;
			const float z = -2.0f;
			const float thalf = x * 0.5f;
			const float tspace = y * 0.2f;
			vbd.EmplaceBack(XMFLOAT3{-x, y, 0.0f});
			vbd.EmplaceBack(XMFLOAT3{x, y, 0.0f});
			vbd.EmplaceBack(XMFLOAT3{x, -y, 0.0f});
			vbd.EmplaceBack(XMFLOAT3{-x, -y, 0.0f});
			vbd.EmplaceBack(XMFLOAT3{0.0f, 0.0f, z});
			vbd.EmplaceBack(XMFLOAT3{-thalf, y + tspace, 0.0f});
			vbd.EmplaceBack(XMFLOAT3{thalf, y + tspace, 0.0f});
			vbd.EmplaceBack(XMFLOAT3{0.0f, y + tspace + thalf, 0.0f});
		}

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

			indices.push_back(0);
			indices.push_back(4);

			indices.push_back(1);
			indices.push_back(4);

			indices.push_back(2);
			indices.push_back(4);

			indices.push_back(3);
			indices.push_back(4);

			indices.push_back(5);
			indices.push_back(6);

			indices.push_back(6);
			indices.push_back(7);

			indices.push_back(7);
			indices.push_back(5);
		}

		pVertices = Bind::VertexBuffer::Resolve(gfx, tag, vbd);
		pIndices = Bind::IndexBuffer::Resolve(gfx, tag, indices);
		pTopology = Bind::Topology::Resolve(gfx, D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

		{
			Rgph::Technique line{Chan::main};
			{
				Rgph::Step only("lambertian");
				{
					auto pvs = Bind::VertexShader::Resolve(gfx, "Solid_VS.cso");
					only.AddBindable(Bind::InputLayout::Resolve(gfx, vbd.GetLayout(), *pvs));
					only.AddBindable(std::move(pvs));

					only.AddBindable(Bind::PixelShader::Resolve(gfx, "Solid_PS.cso"));

					struct PSColorConstant
					{
						XMFLOAT3 color{0.2f, 0.2f, 0.6f};
						float padding{};
					} colorConst;

					only.AddBindable(Bind::PixelConstantBuffer<PSColorConstant>::Resolve(gfx, colorConst, 1u));

					only.AddBindable(std::make_shared<Bind::TransformCBuf>(gfx));

					only.AddBindable(Bind::Rasterizer::Resolve(gfx, CullMode::Back));
				}
				line.AddStep(std::move(only));
			}
			AddTechnique(std::move(line));
		}
	}

	void CameraIndicator::SetPos(XMFLOAT3 pos) noexcept
	{
		this->pos = pos;
	}

	void CameraIndicator::SetRot(XMFLOAT3 rot) noexcept
	{
		this->rot = rot;
	}

	XMMATRIX CameraIndicator::GetTransform() const noexcept
	{
		return XMMatrixRotationRollPitchYawFromVector(XMLoadFloat3(&rot)) *
			XMMatrixTranslationFromVector(XMLoadFloat3(&pos));
	}
}
