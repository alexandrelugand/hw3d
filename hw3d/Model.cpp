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

	void Model::Submit(FrameCommander& frame) const noexcpt
	{
		// I'm still not happy about updating parameters (i.e. mutating a bindable GPU state
		// which is part of a mesh which is part of a node which is part of the model that is
		// const in this call) Can probably do this elsewhere
		pWindow->ApplyParameters();
		pRoot->Submit(frame, XMMatrixIdentity());
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
		/*std::vector<std::shared_ptr<Bind::Bindable>> bindablePtrs;


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

			Dcb::RawLayout lay;
			lay.Add<Dcb::Bool>("normalMapEnabled");
			lay.Add<Dcb::Bool>("specularMapEnabled");
			lay.Add<Dcb::Bool>("hasGlossMap");
			lay.Add<Dcb::Float>("specularPower");
			lay.Add<Dcb::Float3>("specularColor");
			lay.Add<Dcb::Float>("specularMapWeight");

			auto buf = Dcb::Buffer(std::move(lay));
			buf["normalMapEnabled"] = true;
			buf["specularMapEnabled"] = true;
			buf["hasGlossMap"] = hasAlphaGloss;
			buf["specularPower"] = shininess;
			buf["specularColor"] = XMFLOAT3{0.75f, 0.75f, 0.75f};
			buf["specularMapWeight"] = 0.671f;

			bindablePtrs.push_back(std::make_shared<Bind::CachingDynamicPixelCBuf>(gfx, buf, 1u));
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

			Dcb::RawLayout layout;
			layout.Add<Dcb::Float>("specularIntensity");
			layout.Add<Dcb::Float>("specularPower");
			layout.Add<Dcb::Bool>("normalMapEnabled");

			auto cbuf = Dcb::Buffer(std::move(layout));
			cbuf["specularIntensity"] = (specularColor.x + specularColor.y + specularColor.z) / 3.0f;
			cbuf["specularPower"] = shininess;
			cbuf["normalMapEnabled"] = true;

			bindablePtrs.push_back(std::make_shared<Bind::CachingDynamicPixelCBuf>(gfx, cbuf, 1u));
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

			Dcb::RawLayout lay;
			lay.Add<Dcb::Float>("specularPower");
			lay.Add<Dcb::Bool>("hasGloss");
			lay.Add<Dcb::Float>("specularMapWeight");

			auto buf = Dcb::Buffer(std::move(lay));
			buf["specularPower"] = shininess;
			buf["hasGloss"] = hasAlphaGloss;
			buf["specularMapWeight"] = 1.0f;

			bindablePtrs.push_back(std::make_unique<Bind::CachingDynamicPixelCBuf>(gfx, buf, 1u));
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

			Dcb::RawLayout lay;
			lay.Add<Dcb::Float>("specularIntensity");
			lay.Add<Dcb::Float>("specularPower");

			auto buf = Dcb::Buffer(std::move(lay));
			buf["specularIntensity"] = (specularColor.x + specularColor.y + specularColor.z) / 3.0f;
			buf["specularPower"] = shininess;
			buf["specularMapWeight"] = 1.0f;

			bindablePtrs.push_back(std::make_unique<Bind::CachingDynamicPixelCBuf>(gfx, buf, 1u));
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

			Dcb::RawLayout lay;
			lay.Add<Dcb::Float4>("materialColor");
			lay.Add<Dcb::Float4>("specularColor");
			lay.Add<Dcb::Float>("specularPower");

			auto buf = Dcb::Buffer(std::move(lay));
			buf["specularPower"] = shininess;
			buf["specularColor"] = specularColor;
			buf["materialColor"] = diffuseColor;

			bindablePtrs.push_back(std::make_unique<Bind::CachingDynamicPixelCBuf>(gfx, buf, 1u));
		}
		else
		{
			throw std::runtime_error("terrible combination of textures in material mesh");
		}

		// anything with alpha diffuse is 2-sided IN SPONZA, need a better way
		// of signalling 2-sidedness to be more general in the future
		bindablePtrs.push_back(Bind::Rasterizer::Resolve(gfx, hasAlphaDiffuse ? None : Back));
		bindablePtrs.push_back(Bind::Blender::Resolve(gfx, false));
		bindablePtrs.push_back(std::make_shared<Bind::Stencil>(gfx, Bind::Stencil::Mode::Off));

		return std::make_unique<Mesh>(gfx, std::move(bindablePtrs));*/
		return {};
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
