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
			auto pAttribute = pData + descriptor.Resolve<Type>().GetOffset();
			return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute); //Magic of metadata template!
		}

		template <typename T>
		void SetAttributeByIndex(size_t i, T&& val) noexcpt
		{
			const auto& element = descriptor.ResolveByIndex(i);
			auto pAttribute = pData + element.GetOffset();
			switch (element.Type())
			{
			case VertexLayout::Position2D:
				SetAttribute<VertexLayout::Position2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Position3D:
				SetAttribute<VertexLayout::Position3D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Texture2D:
				SetAttribute<VertexLayout::Texture2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Normal:
				SetAttribute<VertexLayout::Normal>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float3Color:
				SetAttribute<VertexLayout::Float3Color>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float4Color:
				SetAttribute<VertexLayout::Float4Color>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::RGBAColor:
				SetAttribute<VertexLayout::RGBAColor>(pAttribute, std::forward<T>(val));
				break;
			default:
				assert("Bad element type" && false);
			}
		}

	protected:
		Vertex(char* pData, const VertexLayout& descriptor)
			: pData(pData), descriptor(descriptor)
		{
			assert(pData != nullptr);
		}

	private:
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
		const VertexLayout& descriptor;
	};
}
