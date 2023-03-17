#include "stdafx.h"
#include "Vertex.h"

namespace Dvtx
{
	Vertex::Vertex(char* pData, const VertexLayout& layout)
		: pData(pData), layout(layout)
	{
		assert(pData != nullptr);
	}
}
