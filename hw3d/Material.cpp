#include "stdafx.h"
#include "Material.h"

using Type = Dvtx::VertexLayout::ElementType;

Material::Material(Graphics& gfx, const aiMaterial& material, const std::filesystem::path& path) noexcpt
	: modelPath(path.string())
{
	const auto rootPath = path.parent_path().string() + "\\";
	{
		aiString tempName;
		material.Get(AI_MATKEY_NAME, tempName);
		name = tempName.C_Str();
	}

	// phong technique
	{
		Rgph::Technique phong{"Phong", Chan::main};
		Rgph::Step step("lambertian");
		std::string shaderCode = "Phong";
		aiString texFileName;

		// common (pre)
		layout.Append(Dvtx::VertexLayout::Position3D);
		layout.Append(Dvtx::VertexLayout::Normal);
		Dcb::RawLayout pscLayout;
		bool hasTexture = false;
		bool hasGlossAlpha = false;

		// diffuse
		{
			bool hasAlpha = false;
			if (material.GetTexture(aiTextureType_DIFFUSE, 0, &texFileName) == aiReturn_SUCCESS)
			{
				hasTexture = true;
				shaderCode += "Dif";
				layout.Append(Dvtx::VertexLayout::Texture2D);
				auto tex = Bind::Texture::Resolve(gfx, rootPath + texFileName.C_Str(), Shaders::Texture::Diffuse);
				if (tex->HasAlpha())
				{
					hasAlpha = true;
					shaderCode += "Msk";
				}
				step.AddBindable(std::move(tex));
			}
			else
			{
				pscLayout.Add<Dcb::Float3>("materialColor");
			}
			step.AddBindable(Bind::Rasterizer::Resolve(gfx, hasAlpha ? CullMode::None : CullMode::Back));
		}
		// specular
		{
			if (material.GetTexture(aiTextureType_SPECULAR, 0, &texFileName) == aiReturn_SUCCESS)
			{
				hasTexture = true;
				shaderCode += "Spc";
				layout.Append(Dvtx::VertexLayout::Texture2D);
				auto tex = Bind::Texture::Resolve(gfx, rootPath + texFileName.C_Str(), Shaders::Texture::Specular);
				hasGlossAlpha = tex->HasAlpha();
				step.AddBindable(std::move(tex));
				pscLayout.Add<Dcb::Bool>("useGlossAlpha");
				pscLayout.Add<Dcb::Bool>("useSpecularMap");
			}
			pscLayout.Add<Dcb::Float3>("specularColor");
			pscLayout.Add<Dcb::Float>("specularWeight");
			pscLayout.Add<Dcb::Float>("specularGloss");
		}
		// normal
		{
			if (material.GetTexture(aiTextureType_NORMALS, 0, &texFileName) == aiReturn_SUCCESS)
			{
				hasTexture = true;
				shaderCode += "Nrm";
				layout.Append(Dvtx::VertexLayout::Texture2D);
				layout.Append(Dvtx::VertexLayout::Tangent);
				layout.Append(Dvtx::VertexLayout::Bitangent);
				step.AddBindable(Bind::Texture::Resolve(gfx, rootPath + texFileName.C_Str(), Shaders::Texture::Normal));
				pscLayout.Add<Dcb::Bool>("useNormalMap");
				pscLayout.Add<Dcb::Float>("normalMapWeight");
			}
		}
		// common (post)
		{
			step.AddBindable(std::make_shared<Bind::TransformCBuf>(gfx, 0u));
			auto pvs = Bind::VertexShader::Resolve(gfx, shaderCode + "_VS.cso");
			step.AddBindable(Bind::InputLayout::Resolve(gfx, layout, *pvs));
			step.AddBindable(std::move(pvs));
			step.AddBindable(Bind::PixelShader::Resolve(gfx, shaderCode + "_PS.cso"));
			if (hasTexture)
			{
				step.AddBindable(Bind::Sampler::Resolve(gfx));
			}

			// PS material params (cbuf)
			Dcb::Buffer buf{std::move(pscLayout)};
			if (auto r = buf["materialColor"]; r.Exists())
			{
				aiColor3D color = {0.45f, 0.45f, 0.85f};
				material.Get(AI_MATKEY_COLOR_DIFFUSE, color);
				r = reinterpret_cast<XMFLOAT3&>(color);
			}

			buf["useGlossAlpha"].SetIfExists(hasGlossAlpha);
			buf["useSpecularMap"].SetIfExists(true);

			if (auto r = buf["specularColor"]; r.Exists())
			{
				aiColor3D color = {0.18f, 0.18f, 0.18f};
				material.Get(AI_MATKEY_COLOR_SPECULAR, color);
				r = reinterpret_cast<XMFLOAT3&>(color);
			}

			buf["specularWeight"].SetIfExists(1.0f);

			if (auto r = buf["specularGloss"]; r.Exists())
			{
				float gloss = 8.0f;
				material.Get(AI_MATKEY_SHININESS, gloss);
				r = gloss;
			}

			buf["useNormalMap"].SetIfExists(true);
			buf["normalMapWeight"].SetIfExists(1.0f);

			step.AddBindable(std::make_unique<Bind::CachingPixelCBuf>(gfx, std::move(buf), Shaders::CBuf::Object));
		}
		phong.AddStep(std::move(step));
		techniques.push_back(std::move(phong));
	}
	// outline technique
	{
		Rgph::Technique outline("Outline", Chan::main, false);
		{
			Rgph::Step mask("outlineMask");

			// TODO: better sub-layout generation tech for future consideration maybe
			mask.AddBindable(Bind::InputLayout::Resolve(gfx, layout, *Bind::VertexShader::Resolve(gfx, "Solid_VS.cso")));

			mask.AddBindable(std::make_shared<Bind::TransformCBuf>(gfx, Shaders::CBuf::Transform));

			// TODO: might need to specify rasterizer when doubled-sided models start being used
			outline.AddStep(std::move(mask));
		}
		{
			Rgph::Step draw("outlineDraw");
			{
				Dcb::RawLayout lay;
				lay.Add<Dcb::Float3>("materialColor");
				auto buf = Dcb::Buffer(std::move(lay));
				buf["materialColor"] = XMFLOAT3{1.0f, 0.4f, 0.4f};
				draw.AddBindable(std::make_shared<Bind::CachingPixelCBuf>(gfx, buf, 2u));
			}

			// TODO: better sub-layout generation tech for future consideration maybe
			draw.AddBindable(Bind::InputLayout::Resolve(gfx, layout, *Bind::VertexShader::Resolve(gfx, "Solid_VS.cso")));

			draw.AddBindable(std::make_shared<Bind::TransformCBuf>(gfx, Shaders::CBuf::Transform));

			// TODO: might need to specify rasterizer when doubled-sided models start being used
			outline.AddStep(std::move(draw));
		}
		techniques.push_back(std::move(outline));
	}
	// shadow map technique
	{
		Rgph::Technique map{"shadowMap", Chan::shadow, true};
		{
			Rgph::Step draw{"shadowMap"};
			{
				// TODO: better sub-layout generation tech for future consideration maybe
				draw.AddBindable(Bind::InputLayout::Resolve(gfx, layout, *Bind::VertexShader::Resolve(gfx, "Solid_VS.cso")));

				draw.AddBindable(std::make_shared<Bind::TransformCBuf>(gfx, Shaders::CBuf::Transform));

				// TODO: might need to specify rasterizer when doubled-sided models start being used
				map.AddStep(std::move(draw));
			}
			techniques.push_back(std::move(map));
		}
	}
}

Dvtx::VertexBufferDescriptor Material::ExtractVertices(const aiMesh& mesh) const noexcept
{
	return {layout, mesh};
}

std::vector<unsigned short> Material::ExtractIndices(const aiMesh& mesh) const noexcept
{
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
	return indices;
}

std::shared_ptr<Bind::VertexBuffer> Material::MakeVertexBindable(Graphics& gfx, const aiMesh& mesh, float scale) const noexcpt
{
	auto vtc = ExtractVertices(mesh);
	if (scale != 1.0f)
	{
		for (auto i = 0u; i < vtc.Size(); i++)
		{
			XMFLOAT3& pos = vtc[i].Attr<Dvtx::VertexLayout::ElementType::Position3D>();
			pos.x *= scale;
			pos.y *= scale;
			pos.z *= scale;
		}
	}
	return Bind::VertexBuffer::Resolve(gfx, MakeMeshTag(mesh), std::move(vtc));
}

std::shared_ptr<Bind::IndexBuffer> Material::MakeIndexBindable(Graphics& gfx, const aiMesh& mesh) const noexcpt
{
	return Bind::IndexBuffer::Resolve(gfx, MakeMeshTag(mesh), ExtractIndices(mesh));
}


std::vector<Rgph::Technique> Material::GetTechniques() const noexcept
{
	return techniques;
}

std::string Material::MakeMeshTag(const aiMesh& mesh) const noexcept
{
	return modelPath + "%" + mesh.mName.C_Str();
}
