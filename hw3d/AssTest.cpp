#include "stdafx.h"
#include "AssTest.h"

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
		VertexBufferDescriptor vbd(
			std::move(VertexDescriptor{}
			          .Append(VertexDescriptor::Position3D)
			          .Append(VertexDescriptor::Normal)
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

		AddStaticIndexBuffer(std::make_unique<IndexBuffer>(gfx, indices));

		AddStaticBind(std::make_unique<VertexBuffer>(gfx, vbd));

		auto pvs = std::make_unique<VertexShader>(gfx, L"PhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		AddStaticBind(std::move(pvs));

		AddStaticBind(std::make_unique<PixelShader>(gfx, L"PhongPS.cso"));

		AddStaticBind(std::make_unique<InputLayout>(gfx, vbd.GetDescriptor().GetLayout(), pvsbc));

		AddStaticBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));
	}
	else
	{
		SetIndexFromStatic();
	}

	AddBind(std::make_unique<TransformCBuf>(gfx, *this));

	materialConstants.color = material;
	AddBind(std::make_unique<MaterialCbuf>(gfx, materialConstants, 1u));
}

void AssTest::SyncMaterial(Graphics& gfx) noexcept(!IS_DEBUG)
{
	auto pConstPS = QueryBindable<MaterialCbuf>();
	assert(pConstPS != nullptr);
	pConstPS->Update(gfx, materialConstants);
}
