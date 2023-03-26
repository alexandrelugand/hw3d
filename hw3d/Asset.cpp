#include "stdafx.h"
#include "Asset.h"

namespace Draw
{
	Asset::Asset(Graphics& gfx, XMFLOAT3 material, float scale, const XMFLOAT3& position)
		: DrawableObject(gfx, scale, position)
	{
		const auto tag = "asset." + Uuid::ToString(Uuid::New());

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

		pVertices = Bind::VertexBuffer::Resolve(gfx, tag, vbd);
		pIndices = Bind::IndexBuffer::Resolve(gfx, tag, indices);
		pTopology = Bind::Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		{
			Technique shade("Shade");
			{
				Step only(0);

				auto pvs = Bind::VertexShader::Resolve(gfx, "BlendedPhong_VS.cso");
				auto pvsbc = pvs->GetBytecode();
				only.AddBindable(std::move(pvs));

				only.AddBindable(Bind::PixelShader::Resolve(gfx, "BlendedPhong_PS.cso"));

				Dcb::RawLayout lay;
				lay.Add<Dcb::Float3>("color");
				lay.Add<Dcb::Float>("specularIntensity");
				lay.Add<Dcb::Float>("specularPower");
				auto buf = Dcb::Buffer(std::move(lay));
				buf["color"] = material;
				buf["specularIntensity"] = 0.1f;
				buf["specularPower"] = 20.0f;
				only.AddBindable(std::make_shared<Bind::CachingPixelCBuf>(gfx, buf, 1u));

				only.AddBindable(std::make_unique<Bind::InputLayout>(gfx, vbd.GetLayout(), pvsbc));
				only.AddBindable(std::make_shared<Bind::TransformCBuf>(gfx));

				shade.AddStep(std::move(only));
			}
			AddTechnique(std::move(shade));
		}
	}

	bool Asset::SpawnControlWindow() noexcept
	{
		bool open = true;
		if (ImGui::Begin(("Asset##"s + std::to_string(id)).c_str(), &open))
		{
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

		return open;
	}
}
