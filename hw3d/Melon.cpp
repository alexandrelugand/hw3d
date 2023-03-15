#include "stdafx.h"
#include "Melon.h"
#include "Sphere.h"

namespace Draw
{
	Melon::Melon(Graphics& gfx,
	             std::mt19937& rng,
	             std::uniform_real_distribution<float>& adist,
	             std::uniform_real_distribution<float>& ddist,
	             std::uniform_real_distribution<float>& odist,
	             std::uniform_real_distribution<float>& rdist,
	             std::uniform_int_distribution<int>& longdist,
	             std::uniform_int_distribution<int>& latdist)
		:
		r(rdist(rng)),
		theta(adist(rng)),
		phi(adist(rng)),
		chi(adist(rng)),
		droll(ddist(rng)),
		dpitch(ddist(rng)),
		dyaw(ddist(rng)),
		dtheta(odist(rng)),
		dphi(odist(rng)),
		dchi(odist(rng))
	{
		if (!IsStaticInitialized())
		{
			auto pvs = std::make_unique<Bind::VertexShader>(gfx, L"ColorIndexVS.cso");
			auto pvsbc = pvs->GetBytecode();
			AddStaticBind(std::move(pvs));

			AddStaticBind(std::make_unique<Bind::PixelShader>(gfx, L"ColorIndexPS.cso"));

			struct PixelShaderConstants
			{
				struct
				{
					float r;
					float g;
					float b;
					float a;
				} face_colors[8];
			};
			const PixelShaderConstants cb =
			{
				{
					{1.0f, 1.0f, 1.0f},
					{1.0f, 0.0f, 0.0f},
					{0.0f, 1.0f, 0.0f},
					{1.0f, 1.0f, 0.0f},
					{0.0f, 0.0f, 1.0f},
					{1.0f, 0.0f, 1.0f},
					{0.0f, 1.0f, 1.0f},
					{0.0f, 0.0f, 0.0f},
				}
			};
			AddStaticBind(std::make_unique<Bind::PixelConstantBuffer<PixelShaderConstants>>(gfx, cb, 0u));

			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			AddStaticBind(std::make_unique<Bind::InputLayout>(gfx, ied, pvsbc));

			AddStaticBind(std::make_unique<Bind::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}

		struct Vertex
		{
			XMFLOAT3 pos;
		};

		auto model = Geometry::Sphere::MakeTesselated<Vertex>(latdist(rng), longdist(rng));

		// deform vertices of model by linear transformation
		model.Transform(XMMatrixScaling(1.0f, 1.0f, 1.2f));

		AddBind(std::make_unique<Bind::VertexBuffer>(gfx, model.vertices));

		AddIndexBuffer(std::make_unique<Bind::IndexBuffer>(gfx, model.indices));

		AddBind(std::make_unique<Bind::TransformCBuf>(gfx, *this));
	}

	void Melon::Update(float dt) noexcept
	{
		roll += droll * dt;
		pitch += dpitch * dt;
		yaw += dyaw * dt;
		theta += dtheta * dt;
		phi += dphi * dt;
		chi += dchi * dt;
	}

	XMMATRIX Melon::GetTransform() const noexcept
	{
		return XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			XMMatrixTranslation(r, 0.0f, 0.0f) *
			XMMatrixRotationRollPitchYaw(theta, phi, chi);
	}
}
