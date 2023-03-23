#include "stdafx.h"
#include "VertexLayout.h"

namespace Dvtx
{
	VertexLayout::Element::Element(ElementType type, size_t offset)
		: type(type), offset(offset)
	{
	}

	size_t VertexLayout::Element::GetOffsetAfter() const noexcpt
	{
		return offset + Size();
	}

	size_t VertexLayout::Element::GetOffset() const
	{
		return offset;
	}

	size_t VertexLayout::Element::Size() const noexcpt
	{
		return SizeOf(type);
	}

	constexpr size_t VertexLayout::Element::SizeOf(ElementType type) noexcpt
	{
		return Bridge<SysSizeLookup>(type);
	}

	VertexLayout::ElementType VertexLayout::Element::GetType() const noexcept
	{
		return type;
	}

	D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GetDesc() const noexcpt
	{
		return Bridge<DescGenerate>(type, GetOffset());
	}

	const char* VertexLayout::Element::GetCode() const noexcept
	{
		return Bridge<CodeLookup>(type);
	}

	const VertexLayout::Element& VertexLayout::ResolveByIndex(size_t index) const noexcpt
	{
		return elements[index];
	}

	VertexLayout& VertexLayout::Append(ElementType type) noexcpt
	{
		elements.emplace_back(type, Size());
		return *this;
	}

	size_t VertexLayout::Size() const noexcpt
	{
		return elements.empty() ? 0u : elements.back().GetOffsetAfter();
	}

	size_t VertexLayout::GetElementCount() const noexcept
	{
		return elements.size();
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> VertexLayout::GetD3DLayout() const noexcpt
	{
		std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
		desc.reserve(GetElementCount());
		for (const auto& e : elements)
		{
			desc.push_back(e.GetDesc());
		}
		return desc;
	}

	std::string VertexLayout::GetCode() const noexcpt
	{
		std::string code;
		for (const auto& e : elements)
		{
			code += e.GetCode();
		}
		return code;
	}
}
