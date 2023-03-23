#pragma once
#include "Forwards.h"

namespace Draw
{
	class Drawable
	{
	public:
		Drawable() = default;
		Drawable(const Drawable&) = delete;
		virtual ~Drawable();

		void AddTechnique(Technique tech_in) noexcept;
		virtual XMMATRIX GetTransform() const noexcept = 0;
		void Submit(FrameCommander& frameCmder) const noexcept;
		void Bind(Graphics& gfx) const noexcept;
		void Accept(TechniqueProbe& probe);
		UINT GetIndexCount() const noexcpt;

	protected:
		std::shared_ptr<Bind::IndexBuffer> pIndices;
		std::shared_ptr<Bind::VertexBuffer> pVertices;
		std::shared_ptr<Bind::Topology> pTopology;
		std::vector<Technique> techniques{};
	};
}
