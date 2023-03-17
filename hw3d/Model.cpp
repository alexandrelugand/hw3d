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

	Model::Model(Graphics& gfx, const std::string fileName)
		: pWindow(std::make_unique<Windowing::ModelWindow>())
	{
		Assimp::Importer imp;
		const auto pScene = imp.ReadFile(fileName.c_str(),
		                                 aiProcess_Triangulate |
		                                 aiProcess_JoinIdenticalVertices |
		                                 aiProcess_ConvertToLeftHanded |
		                                 aiProcess_GenNormals
		);

		if (pScene == nullptr)
		{
			throw ModelException(__LINE__, __FILE__, imp.GetErrorString());
		}

		for (size_t i = 0; i < pScene->mNumMeshes; i++)
		{
			meshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i], pScene->mMaterials));
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

	void Model::ShowWindow(const char* windowName)
	{
		pWindow->Show(windowName, *this, *pRoot);
	}

	std::unique_ptr<Mesh> Model::ParseMesh(Graphics& gfx, const aiMesh& mesh, const aiMaterial* const* pMaterial)
	{
		Dvtx::VertexBufferDescriptor vbd(
			std::move(Dvtx::VertexLayout{}
			          .Append(Dvtx::VertexLayout::Position3D)
			          .Append(Dvtx::VertexLayout::Normal)
			          .Append(Dvtx::VertexLayout::Texture2D)
			)
		);

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbd.EmplaceBack(
				*reinterpret_cast<XMFLOAT3*>(&mesh.mVertices[i]),
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

		std::vector<std::shared_ptr<Bind::Bindable>> bindablePtrs;
		const auto base = "Models\\nano_textured\\"s;

		bool hasSpecularMap = false;
		float defaultShininess = 35.0f;

		if (mesh.mMaterialIndex >= 0)
		{
			auto& material = *pMaterial[mesh.mMaterialIndex];
			aiString texFileName;

			if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
			{
				bindablePtrs.push_back(Bind::Texture::Resolve(gfx, base + texFileName.C_Str()));
			}

			if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
			{
				bindablePtrs.push_back(Bind::Texture::Resolve(gfx, base + texFileName.C_Str(), 1u));
				hasSpecularMap = true;
			}
			else
			{
				material.Get(AI_MATKEY_SHININESS, defaultShininess);
			}

			bindablePtrs.push_back(Bind::Sampler::Resolve(gfx));
		}

		auto meshTag = base + "%" + mesh.mName.C_Str();
		bindablePtrs.push_back(Bind::VertexBuffer::Resolve(gfx, meshTag, vbd));
		bindablePtrs.push_back(Bind::IndexBuffer::Resolve(gfx, meshTag, indices));

		auto pvs = std::make_unique<Bind::VertexShader>(gfx, "PhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));
		bindablePtrs.push_back(Bind::InputLayout::Resolve(gfx, vbd.GetLayout(), pvsbc));

		if (hasSpecularMap)
			bindablePtrs.push_back(Bind::PixelShader::Resolve(gfx, "PhongPSSpecMap.cso"));
		else
			bindablePtrs.push_back(Bind::PixelShader::Resolve(gfx, "PhongPS.cso"));

		struct PSMaterialConstant
		{
			float specularIntensity = 1.6f;
			float specularPower = 30.0f;
			float padding[2]{};
		} pmc;
		pmc.specularPower = defaultShininess;
		bindablePtrs.push_back(Bind::PixelConstantBuffer<PSMaterialConstant>::Resolve(gfx, pmc, 1u));

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
