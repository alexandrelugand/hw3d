#include "stdafx.h"
#include "SolidSphere.h"
#include "Sphere.h"

namespace Draw
{
	SolidSphere::SolidSphere(Graphics& gfx, float radius, XMFLOAT3 color)
		: ColorDrawable(color)
	{
		if (!IsStaticInitialized())
		{
			struct Vertex
			{
				XMFLOAT3 pos;
			};

			auto model = Geometry::Sphere::Make<Vertex>();
			model.Transform(XMMatrixScaling(radius, radius, radius));

			AddStaticBind(std::make_unique<Bind::VertexBuffer>(gfx, model.vertices));
			AddStaticIndexBuffer(std::make_unique<Bind::IndexBuffer>(gfx, model.indices));

			auto pvs = std::make_unique<Bind::VertexShader>(gfx, L"SolidVS.cso");
			auto pvsbc = pvs->GetBytecode();
			AddStaticBind(std::move(pvs));

			AddStaticBind(std::make_unique<Bind::PixelShader>(gfx, L"SolidPS.cso"));

			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};
			AddStaticBind(std::make_unique<Bind::InputLayout>(gfx, ied, pvsbc));

			AddStaticBind(std::make_unique<Bind::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else
		{
			SetIndexFromStatic();
		}

		AddBind(std::make_unique<Bind::TransformCBuf>(gfx, *this));
		AddBind(std::make_unique<Bind::ColorCBuf>(gfx, *this));
	}

	void SolidSphere::Update(float dt) noexcept
	{
	}

	void SolidSphere::SetPos(XMFLOAT3 pos) noexcept
	{
		position = pos;
	}

	XMMATRIX SolidSphere::GetTransform() const noexcept
	{
		return XMMatrixTranslation(position.x, position.y, position.z);
	}
}
