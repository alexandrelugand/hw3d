#include "stdafx.h"
#include "Sheet.h"
#include "Plane.h"

namespace Draw
{
	Sheet::Sheet(Graphics& gfx,
	             std::mt19937& rng,
	             std::uniform_real_distribution<float>& adist,
	             std::uniform_real_distribution<float>& ddist,
	             std::uniform_real_distribution<float>& odist,
	             std::uniform_real_distribution<float>& rdist)
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
			struct Vertex
			{
				XMFLOAT3 pos;

				struct
				{
					float u;
					float v;
				} tex;
			};

			auto model = Geometry::Plane::Make<Vertex>();
			model.vertices[0].tex = {0.0f, 0.0f};
			model.vertices[1].tex = {1.0f, 0.0f};
			model.vertices[2].tex = {0.0f, 1.0f};
			model.vertices[3].tex = {1.0f, 1.0f};

			AddStaticBind(std::make_unique<Bind::VertexBuffer>(gfx, model.vertices));

			AddStaticBind(std::make_unique<Bind::Texture>(gfx, Surface::FromFile("images\\alex.png")));
			AddStaticBind(std::make_unique<Bind::Sampler>(gfx));

			auto pvs = std::make_unique<Bind::VertexShader>(gfx, L"TextureVS.cso");
			auto pvsbc = pvs->GetBytecode();
			AddStaticBind(std::move(pvs));

			AddStaticBind(std::make_unique<Bind::PixelShader>(gfx, L"TexturePS.cso"));

			AddStaticIndexBuffer(std::make_unique<Bind::IndexBuffer>(gfx, model.indices));

			const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
			};
			AddStaticBind(std::make_unique<Bind::InputLayout>(gfx, ied, pvsbc));

			AddStaticBind(std::make_unique<Bind::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else
		{
			SetIndexFromStatic();
		}

		AddBind(std::make_unique<Bind::TransformCBuf>(gfx, *this));
	}

	void Sheet::Update(float dt) noexcept
	{
		roll += droll * dt;
		pitch += dpitch * dt;
		yaw += dyaw * dt;
		theta += dtheta * dt;
		phi += dphi * dt;
		chi += dchi * dt;
	}

	XMMATRIX Sheet::GetTransform() const noexcept
	{
		return XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
			XMMatrixTranslation(r, 0.0f, 0.0f) *
			XMMatrixRotationRollPitchYaw(theta, phi, chi);
	}
}
