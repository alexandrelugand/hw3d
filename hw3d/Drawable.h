#pragma once
#include "Forwards.h"

namespace Draw
{
	class Drawable
	{
	public:
		Drawable() = default;
		Drawable(Graphics& gfx, const Material& mat, const aiMesh& mesh, float scale) noexcept;
		Drawable(const Drawable&) = delete;
		virtual ~Drawable();

		void AddTechnique(Technique tech_in) noexcept;
		virtual XMMATRIX GetTransform() const noexcept = 0;
		void Submit() const noexcept;
		void Bind(Graphics& gfx) const noexcpt;
		void Accept(Probes::TechniqueProbe& probe);
		UINT GetIndexCount() const noexcpt;
		void LinkTechniques(Rgph::RenderGraph& rg);

	protected:
		std::shared_ptr<Bind::IndexBuffer> pIndices;
		std::shared_ptr<Bind::VertexBuffer> pVertices;
		std::shared_ptr<Bind::Topology> pTopology;
		std::vector<Technique> techniques{};
	};
}
