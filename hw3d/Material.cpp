#include "stdafx.h"
#include "Material.h"

using Type = Dvtx::VertexLayout::ElementType;

Material::Material()
{
}

Dvtx::VertexBufferDescriptor Material::ExtractVertices(const aiMesh& mesh) const noexcept
{
	Dvtx::VertexBufferDescriptor vbd{layout};
	vbd.Resize(mesh.mNumVertices);
	if (layout.Has<Type::Position3D>())
	{
		for (unsigned int i = 0; i < mesh.mNumVertices; i++)
		{
			//vbd[i].
		}
	}
	return vbd;
}

std::vector<Technique> Material::GetTechniques() const noexcept
{
	return techniques;
}
