#include "stdafx.h"
#include "Asset.h"

namespace Draw
{
	Asset::Asset(Graphics& gfx, XMFLOAT3 material, float scale)
		: DrawableObject(gfx)
	{
		const auto tag = "asset." + Uuid::ToString(Uuid::New());
		materialConstants.color = material;

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

		AddBind(Bind::VertexBuffer::Resolve(gfx, tag, vbd));
		AddBind(Bind::IndexBuffer::Resolve(gfx, tag, indices));

		const auto pvs = Bind::VertexShader::Resolve(gfx, "BlendedPhongVS.cso");
		const auto pvsbc = pvs->GetBytecode();
		AddBind(std::move(pvs));

		AddBind(Bind::PixelShader::Resolve(gfx, "BlendedPhongPS.cso"));


		AddBind(Bind::InputLayout::Resolve(gfx, vbd.GetLayout(), pvsbc));
		AddBind(Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

		AddBind(std::make_shared<Bind::TransformCBuf>(gfx, *this));
		AddBind(std::make_shared<MaterialCbuf>(gfx, materialConstants, 1u));
	}

	bool Asset::SpawnControlWindow() noexcept
	{
		bool dirty = false;
		bool open = true;
		if (ImGui::Begin("Asset", &open))
		{
			ImGui::Text("Material Properties");
			const auto cd = ImGui::ColorEdit3("Material Color", &materialConstants.color.x);
			const auto sid = ImGui::SliderFloat("Specular Intensity", &materialConstants.specularIntensity, 0.05f, 4.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
			const auto spd = ImGui::SliderFloat("Specular Power", &materialConstants.specularPower, 1.0f, 200.0f, "%.2f", ImGuiSliderFlags_NoRoundToFormat);
			dirty = cd || sid || spd;

			ImGui::Text("Position");
			ImGui::SliderFloat("X", &pos.x, -80.0f, 80.0f, "%.1f");
			ImGui::SliderFloat("Y", &pos.y, -80.0f, 80.0f, "%.1f");
			ImGui::SliderFloat("Z", &pos.z, -80.0f, 80.0f, "%.1f");

			ImGui::Text("Rotation");
			ImGui::SliderAngle("Theta", &theta, -180.0f, 180.0f);
			ImGui::SliderAngle("Phi", &phi, -180.0f, 180.0f);

			ImGui::Text("Orientation");
			ImGui::SliderAngle("Roll", &roll, -180.0f, 180.0f);
			ImGui::SliderAngle("Pitch", &pitch, -180.0f, 180.0f);
			ImGui::SliderAngle("Yaw", &yaw, -180.0f, 180.0f);

			if (ImGui::Button("Reset"))
				Reset();
		}
		ImGui::End();

		if (dirty)
		{
			SyncMaterial();
		}

		return open;
	}

	void Asset::SyncMaterial() noexcpt
	{
		const auto pConstPS = QueryBindable<MaterialCbuf>();
		assert(pConstPS != nullptr);
		pConstPS->Update(gfx, materialConstants);
	}
}
