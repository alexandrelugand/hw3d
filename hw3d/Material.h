#pragma once

class Material
{
public:
	Material();

	Dvtx::VertexBufferDescriptor ExtractVertices(const aiMesh& mesh) const noexcept;
	std::vector<Technique> GetTechniques() const noexcept;

private:
	Dvtx::VertexLayout layout;
	std::vector<Technique> techniques{};
};
