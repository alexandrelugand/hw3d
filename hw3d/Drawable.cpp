#include "stdafx.h"
#include "Drawable.h"

namespace Draw
{
	Drawable::Drawable(Graphics& gfx, const Material& mat, const aiMesh& mesh, float scale) noexcept
	{
		pVertices = mat.MakeVertexBindable(gfx, mesh, scale);
		pIndices = mat.MakeIndexBindable(gfx, mesh);
		pTopology = Bind::Topology::Resolve(gfx);

		for (auto& t : mat.GetTechniques())
		{
			AddTechnique(std::move(t));
		}
	}

	Drawable::~Drawable()
	{
	}

	void Drawable::AddTechnique(Technique tech_in) noexcept
	{
		tech_in.InitializeParentReferences(*this);
		techniques.push_back(std::move(tech_in));
	}

	void Drawable::Submit(FrameCommander& frameCmder) const noexcept
	{
		for (const auto& t : techniques)
		{
			t.Submit(frameCmder, *this);
		}
	}

	void Drawable::Bind(Graphics& gfx) const noexcept
	{
		pTopology->Bind(gfx);
		pVertices->Bind(gfx);
		pIndices->Bind(gfx);
	}

	void Drawable::Accept(TechniqueProbe& probe)
	{
		for (auto& t : techniques)
		{
			t.Accept(probe);
		}
	}

	UINT Drawable::GetIndexCount() const noexcpt
	{
		return pIndices->GetCount();
	}
}
