#include "stdafx.h"
#include "AssTest.h"

namespace Draw
{
	AssTest::AssTest(Graphics& gfx, std::mt19937& rng,
	                 std::uniform_real_distribution<float>& adist,
	                 std::uniform_real_distribution<float>& ddist,
	                 std::uniform_real_distribution<float>& odist,
	                 std::uniform_real_distribution<float>& rdist,
	                 std::uniform_real_distribution<float>& bdist,
	                 XMFLOAT3 material,
	                 float scale)
		: TestObject(gfx, rng, adist, ddist, odist, rdist)
	{
		if (!IsStaticInitialized())
		{
			Dvtx::VertexBufferDescriptor vbd(
				std::move(Dvtx::VertexLayout{}
				          .Append(Dvtx::VertexLayout::Position3D)
				          .Append(Dvtx::VertexLayout::Normal)
				)
			);

			Assimp::Importer imp;
			const auto model = imp.ReadFile("models\\suzanne.obj", aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
			const auto mesh = model->mMeshes[0];

			for (unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				vbd.EmplaceBack(
					XMFLOAT3{mesh->mVertices[i].x * scale, mesh->mVertices[i].y * scale, mesh->mVertices[i].z * scale},
					*reinterpret_cast<XMFLOAT3*>(&mesh->mNormals[i])
				);
			}

			std::vector<unsigned short> indices;
			indices.reserve(mesh->mNumFaces * 3);
			for (unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				const auto& face = mesh->mFaces[i];
				assert(face.mNumIndices == 3);
				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}

			AddStaticIndexBuffer(std::make_unique<Bind::IndexBuffer>(gfx, indices));

			AddStaticBind(std::make_unique<Bind::VertexBuffer>(gfx, vbd));

			auto pvs = std::make_unique<Bind::VertexShader>(gfx, L"PhongVS.cso");
			auto pvsbc = pvs->GetBytecode();
			AddStaticBind(std::move(pvs));

			AddStaticBind(std::make_unique<Bind::PixelShader>(gfx, L"PhongPS.cso"));

			AddStaticBind(std::make_unique<Bind::InputLayout>(gfx, vbd.GetDescriptor().GetLayout(), pvsbc));

			AddStaticBind(std::make_unique<Bind::Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
		}
		else
		{
			SetIndexFromStatic();
		}

		AddBind(std::make_unique<Bind::TransformCBuf>(gfx, *this));

		materialConstants.color = material;
		AddBind(std::make_unique<MaterialCbuf>(gfx, materialConstants, 1u));
	}

	void AssTest::SyncMaterial(Graphics& gfx) noexcept(!IS_DEBUG)
	{
		auto pConstPS = QueryBindable<MaterialCbuf>();
		assert(pConstPS != nullptr);
		pConstPS->Update(gfx, materialConstants);
	}
}
