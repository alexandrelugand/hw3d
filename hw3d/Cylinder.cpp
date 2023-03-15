#include "stdafx.h"
#include "Cylinder.h"
#include "Prism.h"

namespace Draw
{
	Cylinder::Cylinder(Graphics& gfx, std::mt19937& rng,
	                   std::uniform_real_distribution<float>& adist,
	                   std::uniform_real_distribution<float>& ddist,
	                   std::uniform_real_distribution<float>& odist,
	                   std::uniform_real_distribution<float>& rdist,
	                   std::uniform_real_distribution<float>& bdist,
	                   std::uniform_int_distribution<int>& tdist)
		: TestObject(gfx, rng, adist, ddist, odist, rdist)
	{
		if (!IsStaticInitialized())
		{
			auto pvs = std::make_unique<Bind::VertexShader>(gfx, L"PhongVS.cso");
			auto pvsbc = pvs->GetBytecode();
			AddStaticBind(std::move(pvs));

			AddStaticBind(std::make_unique<Bind::PixelShader>(gfx, L"IndexedPhongPS.cso"));

			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			AddStaticBind(std::make_unique<Bind::InputLayout>(gfx, ied, pvsbc));

			AddStaticBind(std::make_unique<Bind::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

			struct PSMaterialConstant
			{
				XMFLOAT3A colors[6] = {
					{1.0f, 0.0f, 0.0f},
					{0.0f, 1.0f, 0.0f},
					{0.0f, 0.0f, 1.0f},
					{1.0f, 1.0f, 0.0f},
					{1.0f, 0.0f, 1.0f},
					{0.0f, 1.0f, 1.0f},
				};
				float specularIntensity = 0.6f;
				float specularPower = 30.0f;
			} matConst;
			AddStaticBind(std::make_unique<Bind::PixelConstantBuffer<PSMaterialConstant>>(gfx, matConst, 1u));
		}

		struct Vertex
		{
			XMFLOAT3 pos;
			XMFLOAT3 n;
		};

		const auto model = Geometry::Prism::MakeTesselatedIndependentCapNormals<Vertex>(tdist(rng));
		AddBind(std::make_unique<Bind::VertexBuffer>(gfx, model.vertices));
		AddIndexBuffer(std::make_unique<Bind::IndexBuffer>(gfx, model.indices));

		AddBind(std::make_unique<Bind::TransformCBuf>(gfx, *this));
	}
}
