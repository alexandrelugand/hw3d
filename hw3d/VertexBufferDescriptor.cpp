#include "stdafx.h"
#include "VertexBufferDescriptor.h"

namespace Dvtx
{
	VertexBufferDescriptor::VertexBufferDescriptor(const VertexLayout& layout_in, size_t size) noexcpt
		: layout(std::move(layout_in))
	{
		Resize(size);
	}

	VertexBufferDescriptor::VertexBufferDescriptor(const VertexLayout& layout_in, const aiMesh& mesh)
		: layout(std::move(layout_in))
	{
		Resize(mesh.mNumVertices);
		for (size_t i = 0, end = layout.GetElementCount(); i < end; i++)
		{
			VertexLayout::Bridge<AttributeAiMeshFill>(layout.ResolveByIndex(i).GetType(), this, mesh);
		}
	}

	void VertexBufferDescriptor::Resize(size_t newSize) noexcpt
	{
		const auto size = Size();
		if (size < newSize)
		{
			buffer.resize(buffer.size() + layout.Size() * (newSize - size));
		}
	}

	const VertexLayout& VertexBufferDescriptor::GetLayout() const noexcept
	{
		return layout;
	}

	size_t VertexBufferDescriptor::Size() const noexcpt
	{
		return buffer.size() / layout.Size();
	}

	size_t VertexBufferDescriptor::SizeBytes() const noexcpt
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
		assert(i < Size());
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
