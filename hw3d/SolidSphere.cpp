#include "stdafx.h"
#include "SolidSphere.h"
#include "Sphere.h"

SolidSphere::SolidSphere(Graphics& gfx, float radius, XMFLOAT3 color)
	: ColorDrawable(color)
{
	if (!IsStaticInitialized())
	{
		struct Vertex
		{
			XMFLOAT3 pos;
		};

		auto model = Sphere::Make<Vertex>();
		model.Transform(XMMatrixScaling(radius, radius, radius));

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, model.vertices));
		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, model.indices));

		auto pvs = std::make_unique<VertexShader>(gfx, L"SolidVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"SolidPS.cso"));

		/*	struct PSColorConstant
			{
				XMFLOAT3 color = {1.0f, 1.0f, 1.0f};
				float padding;
			} colorConst;
			AddStaticBind(std::make_unique<PixelConstantBuffer<PSColorConstant>>(gfx, colorConst, 0u));*/

		const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0}
		};
		AddStaticBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCBuf>(gfx, *this));
	AddBind(std::make_unique<ColorCBuf>(gfx, *this));
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
