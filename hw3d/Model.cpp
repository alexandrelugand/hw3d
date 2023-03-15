#include "stdafx.h"
#include "Model.h"

namespace Entities
{
	ModelException::ModelException(int line, const char* file, std::string note) noexcept
		: Hw3DException(line, file), note(std::move(note))
	{
	}

	const char* ModelException::what() const noexcept
	{
		std::ostringstream oss;
		oss << Hw3DException::what()
			<< "[Note] " << GetNote();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* ModelException::GetType() const noexcept
	{
		return "HW3D Model exception";
	}

	const std::string& ModelException::GetNote() const noexcept
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
			meshPtrs.push_back(ParseMesh(gfx, *pScene->mMeshes[i]));
		}

		pRoot = ParseNode(*pScene->mRootNode);
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

	std::unique_ptr<Mesh> Model::ParseMesh(Graphics& gfx, const aiMesh& mesh)
	{
		Dvtx::VertexBufferDescriptor vbd(
			std::move(Dvtx::VertexLayout{}
			          .Append(Dvtx::VertexLayout::Position3D)
			          .Append(Dvtx::VertexLayout::Normal)
			)
		);

		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			vbd.EmplaceBack(
				*reinterpret_cast<XMFLOAT3*>(&mesh.mVertices[i]),
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

		std::vector<std::unique_ptr<Bind::Bindable>> bindablePtrs;
		bindablePtrs.push_back(std::make_unique<Bind::VertexBuffer>(gfx, vbd));
		bindablePtrs.push_back(std::make_unique<Bind::IndexBuffer>(gfx, indices));

		auto pvs = std::make_unique<Bind::VertexShader>(gfx, L"PhongVS.cso");
		auto pvsbc = pvs->GetBytecode();
		bindablePtrs.push_back(std::move(pvs));

		bindablePtrs.push_back(std::make_unique<Bind::PixelShader>(gfx, L"PhongPS.cso"));

		struct PSMaterialConstant
		{
			XMFLOAT3 color = {0.6f, 0.6f, 0.8f};
			float specularIntensity = 0.6f;
			float specularPower = 30.0f;
			float padding[3];
		} pmc;
		bindablePtrs.push_back(std::make_unique<Bind::PixelConstantBuffer<PSMaterialConstant>>(gfx, pmc, 1u));

		bindablePtrs.push_back(std::make_unique<Bind::InputLayout>(gfx, vbd.GetDescriptor().GetLayout(), pvsbc));

		return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));
	}

	std::unique_ptr<Node> Model::ParseNode(const aiNode& node)
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

		auto pNode = std::make_unique<Node>(node.mName.C_Str(), std::move(curMeshPtrs), transform);
		for (size_t i = 0; i < node.mNumChildren; i++)
		{
			pNode->AddChild(ParseNode(*node.mChildren[i]));
		}

		return pNode;
	}
}
