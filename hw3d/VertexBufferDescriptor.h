#pragma once
#include "ConstVertex.h"

namespace Dvtx
{
	class VertexBufferDescriptor
	{
	public:
		VertexBufferDescriptor(const VertexLayout& layout, size_t size = 0u) noexcpt;
		VertexBufferDescriptor(const VertexLayout& layout, const aiMesh& mesh);

		void Resize(size_t newSize) noexcpt;
		const VertexLayout& GetLayout() const noexcept;
		size_t Size() const noexcpt;
		size_t SizeBytes() const noexcpt;
		const char* GetData() const noexcpt;

		template <VertexLayout::ElementType type>
		struct AttributeAiMeshFill
		{
			static constexpr void Exec(VertexBufferDescriptor* pBuf, const aiMesh& mesh) noexcpt
			{
				for (auto end = mesh.mNumVertices, i = 0u; i < end; i++)
				{
					(*pBuf)[i].Attr<type>() = VertexLayout::Map<type>::Extract(mesh, i);
				}
			}
		};

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
