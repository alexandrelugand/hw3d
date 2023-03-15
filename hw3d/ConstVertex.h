#pragma once
#include "Vertex.h"

namespace Dvtx
{
	class ConstVertex
	{
	public:
		ConstVertex(const Vertex& vertex) noexcpt
			: vertex(vertex)
		{
		}

		template <VertexLayout::ElementType Type>
		const auto& Attr() const noexcpt
		{
			return const_cast<Vertex&>(vertex).Attr<Type>();
		}

	private:
		Vertex vertex;
	};
}
