#include "stdafx.h"
#include "Model.h"
// ReSharper disable CppUnsignedZeroComparison

namespace Entities
{
	Model::ModelException::ModelException(int line, const char* file, std::string note) noexcept
		: Hw3DException(line, file), note(std::move(note))
	{
	}

	const char* Model::ModelException::what() const noexcept
	{
		std::ostringstream oss;
		oss << Hw3DException::what()
			<< "[Note] " << GetNote();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* Model::ModelException::GetType() const noexcept
	{
		return "HW3D Model exception";
	}

	const std::string& Model::ModelException::GetNote() const noexcept
	{
		return note;
	}

	Model::Model(Graphics& gfx, const std::string& filePath, const float& scale)
		: pWindow(std::make_unique<Windowing::ModelWindow>())
	{
		Assimp::Importer imp;
		const auto pScene = imp.ReadFile(filePath.c_str(),
		                                 aiProcess_Triangulate |
		                                 aiProcess_JoinIdenticalVertices |
		                                 aiProcess_ConvertToLeftHanded |
		                                 aiProcess_GenNormals |
		                                 aiProcess_CalcTangentSpace
		);

		if (pScene == nullptr)
		{
			throw ModelException(__LINE__, __FILE__, imp.GetErrorString());
		}

		const auto path = std::filesystem::path(filePath);
		const auto& base = String::trim(path.parent_path().string(), '\\') + "\\"s;
		for (size_t i = 0; i < pScene->mNumMeshes; i++)
		{
			meshPtrs.push_back(ParseMesh(gfx, base, *pScene->mMeshes[i], pScene->mMaterials, scale));
		}

		int nextId = 0;
		pRoot = ParseNode(nextId, *pScene->mRootNode);
	}

	Model::~Model() noexcept
	{
	}

	void Model::Draw(Graphics& gfx) const noexcpt
	{
		if (auto node = pWindow->GetSelectedNode())
		{
			node->SetAppliedTransform(pWindow->GetTransform());
		}
		pRoot->Draw(gfx, XMMatrixIdentity());
	}

	void Model::ShowWindow(Graphics& gfx, const char* windowName)
	{
		pWindow->Show(gfx, windowName, *this, *pRoot);
	}

	void Model::SetRootTransform(FXMMATRIX tf) const noexcept
	{
		pRoot->SetAppliedTransform(tf);
	}

	std::unique_ptr<Mesh> Model::ParseMesh(Graphics& gfx, const std::string& base, const aiMesh& mesh, const aiMaterial* const* pMaterials, const float& scale)
	{
		std::vector<std::shared_ptr<Bind::Bindable>> bindablePtrs;


		bool hasSpecularMap = false;
		bool hasAlphaGloss = false;
		bool hasAlphaDiffuse = false;
		bool hasNormalMap = false;
		bool hasDiffuseMap = false;
		float shininess = 2.0f;

		XMFLOAT4 diffuseColor = {0.45f, 0.45f, 0.85f, 1.0f};
		XMFLOAT4 specularColor = {0.18f, 0.18f, 0.18f, 1.0f};

		if (mesh.mMaterialIndex >= 0)
		{
			auto& material = *pMaterials[mesh.mMaterialIndex];

			aiString texFileName;

			if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
			{
				bindablePtrs.push_back(Bind::Texture::Resolve(gfx, base + texFileName.C_Str()));
				auto tex = Bind::Texture::Resolve(gfx, base + texFileName.C_Str());
				hasAlphaDiffuse = tex->HasAlpha();
				bindablePtrs.push_back(std::move(tex));
				hasDiffuseMap = true;
			}
			else
			{
				material.Get(AI_MATKEY_COLOR_DIFFUSE, reinterpret_cast<aiColor3D&>(diffuseColor));
			}

			if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
			{
				auto tex = Bind::Texture::Resolve(gfx, base + texFileName.C_Str(), 1);
				hasAlphaGloss = tex->HasAlpha();
				bindablePtrs.push_back(std::move(tex));
				hasSpecularMap = true;
			}
			else
			{
				material.Get(AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor3D&>(specularColor));
			}

			if (!hasAlphaGloss)
			{
				material.Get(AI_MATKEY_SHININESS, shininess);
			}

			if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
			{
				auto tex = Bind::Texture::Resolve(gfx, base + texFileName.C_Str(), 2);
				hasAlphaGloss = tex->HasAlpha();
				bindablePtrs.push_back(std::move(tex));
				hasNormalMap = true;
			}

			if (hasDiffuseMap || hasSpecularMap || hasNormalMap)
			{
				bindablePtrs.push_back(Bind::Sampler::Resolve(gfx));
			}
		}

		const auto meshTag = base + "%" + mesh.mName.C_Str();

		if (hasDiffuseMap && hasNormalMap && hasSpecularMap)
		{
			Dvtx::VertexBufferDescriptor vbd(std::move(
				Dvtx::VertexLayout{}
				.Append(Dvtx::VertexLayout::Position3D)
				.Append(Dvtx::VertexLayout::Normal)
				.Append(Dvtx::VertexLayout::Tangent)
				.Append(Dvtx::VertexLayout::Bitangent)
				.Append(Dvtx::VertexLayout::Texture2D)
			));

			for (unsigned int i = 0; i < mesh.mNumVertices; i++)
			{
				vbd.EmplaceBack(
					XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
					*reinterpret_cast<XMFLOAT3*>(&mesh.mNormals[i]),
					*reinterpret_cast<XMFLOAT3*>(&mesh.mTangents[i]),
					*reinterpret_cast<XMFLOAT3*>(&mesh.mBitangents[i]),
					*reinterpret_cast<XMFLOAT2*>(&mesh.mTextureCoords[0][i])
				);
			}

			std::vector<unsigned short> indices;
			indices.reserve(mesh.mNumFaces * 3);
			for (unsigned int i = 0; i < mesh.mNumFaces; i++)
			{
				const auto& face = mesh.mFaces[i];
				assert(face.mNumIndices == 3);
				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}

			bindablePtrs.push_back(Bind::VertexBuffer::Resolve(gfx, meshTag, vbd));

			bindablePtrs.push_back(Bind::IndexBuffer::Resolve(gfx, meshTag, indices));

			auto pvs = Bind::VertexShader::Resolve(gfx, "PhongVSNormalMap.cso");
			auto pvsbc = pvs->GetBytecode();
			bindablePtrs.push_back(std::move(pvs));

			bindablePtrs.push_back(Bind::PixelShader::Resolve(gfx, hasAlphaDiffuse ? "PhongPSSpecNormMask.cso" : "PhongPSSpecNormalMap.cso"));

			bindablePtrs.push_back(Bind::InputLayout::Resolve(gfx, vbd.GetLayout(), pvsbc));

			Renderer::PSMaterialConstantFull pmc{};
			pmc.specularPower = shininess;
			pmc.hasGlossMap = hasAlphaGloss ? TRUE : FALSE;
			// this is CLEARLY an issue... all meshes will share same mat const, but may have different
			// Ns (specular power) specified for each in the material properties... bad conflict
			bindablePtrs.push_back(Bind::PixelConstantBuffer<Renderer::PSMaterialConstantFull>::Resolve(gfx, pmc, 1u));
		}
		else if (hasDiffuseMap && hasNormalMap)
		{
			Dvtx::VertexBufferDescriptor vbd(std::move(
				Dvtx::VertexLayout{}
				.Append(Dvtx::VertexLayout::Position3D)
				.Append(Dvtx::VertexLayout::Normal)
				.Append(Dvtx::VertexLayout::Tangent)
				.Append(Dvtx::VertexLayout::Bitangent)
				.Append(Dvtx::VertexLayout::Texture2D)
			));

			for (unsigned int i = 0; i < mesh.mNumVertices; i++)
			{
				vbd.EmplaceBack(
					XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
					*reinterpret_cast<XMFLOAT3*>(&mesh.mNormals[i]),
					*reinterpret_cast<XMFLOAT3*>(&mesh.mTangents[i]),
					*reinterpret_cast<XMFLOAT3*>(&mesh.mBitangents[i]),
					*reinterpret_cast<XMFLOAT2*>(&mesh.mTextureCoords[0][i])
				);
			}

			std::vector<unsigned short> indices;
			indices.reserve(mesh.mNumFaces * 3);
			for (unsigned int i = 0; i < mesh.mNumFaces; i++)
			{
				const auto& face = mesh.mFaces[i];
				assert(face.mNumIndices == 3);
				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}

			bindablePtrs.push_back(Bind::VertexBuffer::Resolve(gfx, meshTag, vbd));

			bindablePtrs.push_back(Bind::IndexBuffer::Resolve(gfx, meshTag, indices));

			auto pvs = Bind::VertexShader::Resolve(gfx, "PhongVSNormalMap.cso");
			auto pvsbc = pvs->GetBytecode();
			bindablePtrs.push_back(std::move(pvs));

			bindablePtrs.push_back(Bind::PixelShader::Resolve(gfx, "PhongPSNormalMap.cso"));

			bindablePtrs.push_back(Bind::InputLayout::Resolve(gfx, vbd.GetLayout(), pvsbc));

			Renderer::PSMaterialConstantDiffNorm pmc{};
			pmc.specularPower = shininess;
			pmc.specularIntensity = (specularColor.x + specularColor.y + specularColor.z) / 3.0f;
			// this is CLEARLY an issue... all meshes will share same mat const, but may have different
			// Ns (specular power) specified for each in the material properties... bad conflict
			bindablePtrs.push_back(Bind::PixelConstantBuffer<Renderer::PSMaterialConstantDiffNorm>::Resolve(gfx, pmc, 1u));
		}
		else if (hasDiffuseMap && !hasNormalMap && hasSpecularMap)
		{
			Dvtx::VertexBufferDescriptor vbuf(std::move(
				Dvtx::VertexLayout{}
				.Append(Dvtx::VertexLayout::Position3D)
				.Append(Dvtx::VertexLayout::Normal)
				.Append(Dvtx::VertexLayout::Texture2D)
			));

			for (unsigned int i = 0; i < mesh.mNumVertices; i++)
			{
				vbuf.EmplaceBack(
					XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
					*reinterpret_cast<XMFLOAT3*>(&mesh.mNormals[i]),
					*reinterpret_cast<XMFLOAT2*>(&mesh.mTextureCoords[0][i])
				);
			}

			std::vector<unsigned short> indices;
			indices.reserve(mesh.mNumFaces * 3);
			for (unsigned int i = 0; i < mesh.mNumFaces; i++)
			{
				const auto& face = mesh.mFaces[i];
				assert(face.mNumIndices == 3);
				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}

			bindablePtrs.push_back(Bind::VertexBuffer::Resolve(gfx, meshTag, vbuf));

			bindablePtrs.push_back(Bind::IndexBuffer::Resolve(gfx, meshTag, indices));

			auto pvs = Bind::VertexShader::Resolve(gfx, "PhongVS.cso");
			auto pvsbc = pvs->GetBytecode();
			bindablePtrs.push_back(std::move(pvs));

			bindablePtrs.push_back(Bind::PixelShader::Resolve(gfx, "PhongPSSpec.cso"));

			bindablePtrs.push_back(Bind::InputLayout::Resolve(gfx, vbuf.GetLayout(), pvsbc));

			Renderer::PSMaterialConstantDiffuseSpec pmc{};
			pmc.specularPowerConst = shininess;
			pmc.hasGloss = hasAlphaGloss ? TRUE : FALSE;
			pmc.specularMapWeight = 1.0f;
			// this is CLEARLY an issue... all meshes will share same mat const, but may have different
			// Ns (specular power) specified for each in the material properties... bad conflict
			bindablePtrs.push_back(Bind::PixelConstantBuffer<Renderer::PSMaterialConstantDiffuseSpec>::Resolve(gfx, pmc, 1u));
		}
		else if (hasDiffuseMap)
		{
			Dvtx::VertexBufferDescriptor vbd(std::move(
				Dvtx::VertexLayout{}
				.Append(Dvtx::VertexLayout::Position3D)
				.Append(Dvtx::VertexLayout::Normal)
				.Append(Dvtx::VertexLayout::Texture2D)
			));

			for (unsigned int i = 0; i < mesh.mNumVertices; i++)
			{
				vbd.EmplaceBack(
					XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
					*reinterpret_cast<XMFLOAT3*>(&mesh.mNormals[i]),
					*reinterpret_cast<XMFLOAT2*>(&mesh.mTextureCoords[0][i])
				);
			}

			std::vector<unsigned short> indices;
			indices.reserve(mesh.mNumFaces * 3);
			for (unsigned int i = 0; i < mesh.mNumFaces; i++)
			{
				const auto& face = mesh.mFaces[i];
				assert(face.mNumIndices == 3);
				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}

			bindablePtrs.push_back(Bind::VertexBuffer::Resolve(gfx, meshTag, vbd));

			bindablePtrs.push_back(Bind::IndexBuffer::Resolve(gfx, meshTag, indices));

			auto pvs = Bind::VertexShader::Resolve(gfx, "PhongVS.cso");
			auto pvsbc = pvs->GetBytecode();
			bindablePtrs.push_back(std::move(pvs));

			bindablePtrs.push_back(Bind::PixelShader::Resolve(gfx, "PhongPS.cso"));

			bindablePtrs.push_back(Bind::InputLayout::Resolve(gfx, vbd.GetLayout(), pvsbc));

			Renderer::PSMaterialConstantDiffuse pmc{};
			pmc.specularPower = shininess;
			pmc.specularIntensity = (specularColor.x + specularColor.y + specularColor.z) / 3.0f;
			// this is CLEARLY an issue... all meshes will share same mat const, but may have different
			// Ns (specular power) specified for each in the material properties... bad conflict
			bindablePtrs.push_back(Bind::PixelConstantBuffer<Renderer::PSMaterialConstantDiffuse>::Resolve(gfx, pmc, 1u));
		}
		else if (!hasDiffuseMap && !hasNormalMap && !hasSpecularMap)
		{
			Dvtx::VertexBufferDescriptor vbd(std::move(
				Dvtx::VertexLayout{}
				.Append(Dvtx::VertexLayout::Position3D)
				.Append(Dvtx::VertexLayout::Normal)
			));

			for (unsigned int i = 0; i < mesh.mNumVertices; i++)
			{
				vbd.EmplaceBack(
					XMFLOAT3(mesh.mVertices[i].x * scale, mesh.mVertices[i].y * scale, mesh.mVertices[i].z * scale),
					*reinterpret_cast<XMFLOAT3*>(&mesh.mNormals[i])
				);
			}

			std::vector<unsigned short> indices;
			indices.reserve(mesh.mNumFaces * 3);
			for (unsigned int i = 0; i < mesh.mNumFaces; i++)
			{
				const auto& face = mesh.mFaces[i];
				assert(face.mNumIndices == 3);
				indices.push_back(face.mIndices[0]);
				indices.push_back(face.mIndices[1]);
				indices.push_back(face.mIndices[2]);
			}

			bindablePtrs.push_back(Bind::VertexBuffer::Resolve(gfx, meshTag, vbd));

			bindablePtrs.push_back(Bind::IndexBuffer::Resolve(gfx, meshTag, indices));

			auto pvs = Bind::VertexShader::Resolve(gfx, "PhongVSNoTex.cso");
			auto pvsbc = pvs->GetBytecode();
			bindablePtrs.push_back(std::move(pvs));

			bindablePtrs.push_back(Bind::PixelShader::Resolve(gfx, "PhongPSNoTex.cso"));

			bindablePtrs.push_back(Bind::InputLayout::Resolve(gfx, vbd.GetLayout(), pvsbc));

			Renderer::PSMaterialConstantNoTex pmc{};
			pmc.specularPower = shininess;
			pmc.specularColor = specularColor;
			pmc.materialColor = diffuseColor;
			// this is CLEARLY an issue... all meshes will share same mat const, but may have different
			// Ns (specular power) specified for each in the material properties... bad conflict
			bindablePtrs.push_back(Bind::PixelConstantBuffer<Renderer::PSMaterialConstantNoTex>::Resolve(gfx, pmc, 1u));
		}
		else
		{
			throw std::runtime_error("terrible combination of textures in material mesh");
		}

		// anything with alpha diffuse is 2-sided IN SPONZA, need a better way
		// of signalling 2-sidedness to be more general in the future
		bindablePtrs.push_back(Bind::Rasterizer::Resolve(gfx, hasAlphaDiffuse ? None : Back));

		bindablePtrs.push_back(Bind::Blender::Resolve(gfx, false));

		return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));
	}

	std::unique_ptr<Node> Model::ParseNode(int& nextId, const aiNode& node)
	{
		const auto transform = XMMatrixTranspose(
			XMLoadFloat4x4(reinterpret_cast<const XMFLOAT4X4*>(&node.mTransformation))
		);

		std::vector<Mesh*> curMeshPtrs;
		curMeshPtrs.reserve(node.mNumMeshes);
		for (size_t i = 0; i < node.mNumMeshes; i++)
		{
			const auto meshIdx = node.mMeshes[i];
			curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
		}

		auto pNode = std::make_unique<Node>(nextId++, node.mName.C_Str(), std::move(curMeshPtrs), transform);
		for (size_t i = 0; i < node.mNumChildren; i++)
		{
			pNode->AddChild(ParseNode(nextId, *node.mChildren[i]));
		}

		return pNode;
	}
}
