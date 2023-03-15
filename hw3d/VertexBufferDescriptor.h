#pragma once
#include "ConstVertex.h"

namespace Dvtx
{
	class VertexBufferDescriptor
	{
	public:
		VertexBufferDescriptor(VertexLayout descriptor) noexcpt
			: descriptor(descriptor)
		{
		}

		const VertexLayout& GetDescriptor() const noexcept
		{
			return descriptor;
		}

		size_t NumVertices() const noexcpt
		{
			return buffer.size() / descriptor.Size();
		}

		size_t Size() const noexcpt
		{
			return buffer.size();
		}

		const char* GetData() const noexcpt
		{
			return buffer.data();
		}

		template <typename... Params>
		void EmplaceBack(Params&&... params) noexcpt
		{
			assert(sizeof...(params) == descriptor.GetElementCount() && "Param count doesn't match number of vertext elements");
			buffer.resize(buffer.size() + descriptor.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}

		Vertex Back() noexcpt
		{
			assert(buffer.size() != 0u);
			return Vertex{buffer.data() + buffer.size() - descriptor.Size(), descriptor};
		}

		Vertex Front() noexcpt
		{
			assert(buffer.size() != 0u);
			return Vertex{buffer.data(), descriptor};
		}

		Vertex operator[](size_t i) noexcpt
		{
			assert(i < NumVertices());
			return Vertex{buffer.data() + i * descriptor.Size(), descriptor};
		}

		//Const vertex
		ConstVertex Back() const noexcpt
		{
			return const_cast<VertexBufferDescriptor*>(this)->Back();
		}

		ConstVertex Front() const noexcpt
		{
			return const_cast<VertexBufferDescriptor*>(this)->Front();
		}

		ConstVertex operator[](size_t i) const noexcpt
		{
			return const_cast<VertexBufferDescriptor&>(*this)[i];
		}

	private:
		std::vector<char> buffer;
		VertexLayout descriptor;
	};
}
