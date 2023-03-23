#pragma once
#include "ConstVertex.h"

namespace Dvtx
{
	class VertexBufferDescriptor
	{
	public:
		VertexBufferDescriptor(VertexLayout layout, size_t size = 0u) noexcpt;
		void Resize(size_t newSize) noexcpt;
		const VertexLayout& GetLayout() const noexcept;
		size_t NumVertices() const noexcpt;
		size_t Size() const noexcpt;
		const char* GetData() const noexcpt;

		template <typename... Params>
		void EmplaceBack(Params&&... params) noexcpt
		{
			assert(sizeof...(params) == layout.GetElementCount() && "Param count doesn't match number of vertex elements");
			buffer.resize(buffer.size() + layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}

		Vertex Back() noexcpt;
		Vertex Front() noexcpt;
		Vertex operator[](size_t i) noexcpt;

		//Const vertex
		ConstVertex Back() const noexcpt;
		ConstVertex Front() const noexcpt;
		ConstVertex operator[](size_t i) const noexcpt;

	private:
		std::vector<char> buffer{};
		VertexLayout layout;
	};
}
