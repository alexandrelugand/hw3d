#include "stdafx.h"
#include "VertexBufferDescriptor.h"

namespace Dvtx
{
	VertexBufferDescriptor::VertexBufferDescriptor(VertexLayout layout) noexcpt
		: layout(layout)
	{
	}

	const VertexLayout& VertexBufferDescriptor::GetLayout() const noexcept
	{
		return layout;
	}

	size_t VertexBufferDescriptor::NumVertices() const noexcpt
	{
		return buffer.size() / layout.Size();
	}

	size_t VertexBufferDescriptor::Size() const noexcpt
	{
		return buffer.size();
	}

	const char* VertexBufferDescriptor::GetData() const noexcpt
	{
		return buffer.data();
	}

	Vertex VertexBufferDescriptor::Back() noexcpt
	{
		assert(buffer.size() != 0u);
		return Vertex{buffer.data() + buffer.size() - layout.Size(), layout};
	}

	Vertex VertexBufferDescriptor::Front() noexcpt
	{
		assert(buffer.size() != 0u);
		return Vertex{buffer.data(), layout};
	}

	Vertex VertexBufferDescriptor::operator[](size_t i) noexcpt
	{
		assert(i < NumVertices());
		return Vertex{buffer.data() + i * layout.Size(), layout};
	}

	ConstVertex VertexBufferDescriptor::Back() const noexcpt
	{
		return const_cast<VertexBufferDescriptor*>(this)->Back();
	}

	ConstVertex VertexBufferDescriptor::Front() const noexcpt
	{
		return const_cast<VertexBufferDescriptor*>(this)->Front();
	}

	ConstVertex VertexBufferDescriptor::operator[](size_t i) const noexcpt
	{
		return const_cast<VertexBufferDescriptor&>(*this)[i];
	}
}
