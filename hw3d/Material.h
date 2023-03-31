#pragma once
#include "VertexLayout.h"

class Material
{
public:
	Material(Graphics& gfx, const aiMaterial& material, const std::filesystem::path& path) noexcpt;

	Dvtx::VertexBufferDescriptor ExtractVertices(const aiMesh& mesh) const noexcept;
	std::vector<unsigned short> ExtractIndices(const aiMesh& mesh) const noexcept;

	std::shared_ptr<Bind::VertexBuffer> MakeVertexBindable(Graphics& gfx, const aiMesh& mesh, float scale = 1.0f) const noexcpt;
	std::shared_ptr<Bind::IndexBuffer> MakeIndexBindable(Graphics& gfx, const aiMesh& mesh) const noexcpt;

	std::vector<Rgph::Technique> GetTechniques() const noexcept;

private:
	std::string MakeMeshTag(const aiMesh& mesh) const noexcept;

	Dvtx::VertexLayout layout;
	std::vector<Rgph::Technique> techniques{};
	std::string modelPath;
	std::string name;
};
