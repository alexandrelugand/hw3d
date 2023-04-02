#include "stdafx.h"
#include "Model.h"
// ReSharper disable CppUnsignedZeroComparison

namespace Entities
{
	Model::Model(Graphics& gfx, const std::string& name, const std::string& filePath, const float& scale, const XMFLOAT3& position, const XMFLOAT3& rotation)
		: id(GetNextId()), name(name)
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

		// parse materials
		std::vector<Material> materials;
		materials.reserve(pScene->mNumMaterials);
		for (size_t i = 0; i < pScene->mNumMaterials; i++)
		{
			materials.emplace_back(gfx, *pScene->mMaterials[i], filePath);
		}

		for (size_t i = 0; i < pScene->mNumMeshes; i++)
		{
			const auto& mesh = *pScene->mMeshes[i];
			meshPtrs.push_back(std::make_unique<Mesh>(gfx, materials[mesh.mMaterialIndex], mesh, scale));
		}

		int nodeId = 0;
		pRoot = ParseNode(nodeId, *pScene->mRootNode, scale);

		SetRootTransform(XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z) * XMMatrixTranslation(position.x, position.y, position.z));
	}

	Model::~Model() noexcept
	{
	}

	int Model::GetId() const noexcept
	{
		return id;
	}

	const std::string& Model::GetName() const noexcept
	{
		return name;
	}

	void Model::Submit(size_t channelFilter) const noexcpt
	{
		pRoot->Submit(channelFilter, XMMatrixIdentity());
	}

	void Model::SetRootTransform(FXMMATRIX tf) const noexcept
	{
		pRoot->SetAppliedTransform(tf);
	}

	void Model::Accept(Probes::ModelProbe& probe) const
	{
		pRoot->Accept(probe);
	}

	void Model::LinkTechniques(Rgph::RenderGraph& rg) const
	{
		for (const auto& pMesh : meshPtrs)
		{
			pMesh->LinkTechniques(rg);
		}
	}

	std::unique_ptr<Node> Model::ParseNode(int& nodeId, const aiNode& node, float scale)
	{
		const auto transform = ScaleTranslation(XMMatrixTranspose(XMLoadFloat4x4(
			                                        reinterpret_cast<const XMFLOAT4X4*>(&node.mTransformation)
		                                        )), scale);

		std::vector<Mesh*> curMeshPtrs;
		curMeshPtrs.reserve(node.mNumMeshes);
		for (size_t i = 0; i < node.mNumMeshes; i++)
		{
			const auto meshIdx = node.mMeshes[i];
			curMeshPtrs.push_back(meshPtrs.at(meshIdx).get());
		}

		auto pNode = std::make_unique<Node>(nodeId++, node.mName.C_Str(), std::move(curMeshPtrs), transform);
		for (size_t i = 0; i < node.mNumChildren; i++)
		{
			pNode->AddChild(ParseNode(nodeId, *node.mChildren[i], scale));
		}

		return pNode;
	}
}
