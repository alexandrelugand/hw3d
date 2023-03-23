#pragma once
#include "VertexLayout.h"

namespace Dvtx
{
	class Vertex
	{
		friend class VertexBufferDescriptor;

	public:
		template <VertexLayout::ElementType Type>
		auto& Attr() noexcpt
		{
			auto pAttribute = pData + layout.Resolve<Type>().GetOffset();
			return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute); //Magic of metadata template!
		}

		template <typename T>
		void SetAttributeByIndex(size_t i, T&& val) noexcpt
		{
			const auto& element = layout.ResolveByIndex(i);
			auto pAttribute = pData + element.GetOffset();
			VertexLayout::Bridge<AttributeSetting>(
				element.GetType(), this, pAttribute, std::forward<T>(val)
			);
		}

	protected:
		Vertex(char* pData, const VertexLayout& layout);

	private:
		template <VertexLayout::ElementType type>
		struct AttributeSetting
		{
			template <typename T>
			static constexpr auto Exec(Vertex* pVertex, char* pAttribute, T&& val) noexcpt
			{
				return pVertex->SetAttribute<type>(pAttribute, std::forward<T>(val));
			}
		};


		//Enables parameter pack setting of multiple parameters by element index
		template <typename First, typename... Rest>
		void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest) noexcpt
		{
			SetAttributeByIndex(i, std::forward<First>(first));
			SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...); //Recursive call to parse arguments
		}

		// Helper to reduce code duplication in SetAttributeByIndex
		template <VertexLayout::ElementType DestElemType, typename Src>
		void SetAttribute(char* pAttribute, Src&& val) noexcpt
		{
			using Dest = typename VertexLayout::Map<DestElemType>::SysType;
			if constexpr (std::is_assignable_v<Dest, Src>)
			{
				*reinterpret_cast<Dest*>(pAttribute) = val;
			}
			else
			{
				assert("Parameter attribute type mismatch" && false);
			}
		}

		char* pData = nullptr;
		const VertexLayout& layout;
	};
}
