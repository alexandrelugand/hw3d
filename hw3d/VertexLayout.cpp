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

	VertexLayout::ElementType VertexLayout::Element::Type() const noexcpt
	{
		return type;
	}

	D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GetDesc() const noexcpt
	{
		switch (type)
		{
		case Position2D: return GenerateDesc<Position2D>(GetOffset());
		case Position3D: return GenerateDesc<Position3D>(GetOffset());
		case Texture2D: return GenerateDesc<Texture2D>(GetOffset());
		case Normal: return GenerateDesc<Normal>(GetOffset());
		case Tangent: return GenerateDesc<Tangent>(GetOffset());
		case Bitangent: return GenerateDesc<Bitangent>(GetOffset());
		case Float3Color: return GenerateDesc<Float3Color>(GetOffset());
		case Float4Color: return GenerateDesc<Float4Color>(GetOffset());
		case RGBAColor: return GenerateDesc<RGBAColor>(GetOffset());
		default: ;
			assert("Invalid element type" && false);
		}
		return {"INVALID", 0, DXGI_FORMAT_UNKNOWN, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0};
	}

	const char* VertexLayout::Element::GetCode() const noexcept
	{
		switch (type)
		{
		case Position2D:
			return Map<Position2D>::code;
		case Position3D:
			return Map<Position3D>::code;
		case Texture2D:
			return Map<Texture2D>::code;
		case Normal:
			return Map<Normal>::code;
		case Tangent:
			return Map<Tangent>::code;
		case Bitangent:
			return Map<Bitangent>::code;
		case Float3Color:
			return Map<Float3Color>::code;
		case Float4Color:
			return Map<Float4Color>::code;
		case RGBAColor:
			return Map<RGBAColor>::code;
		}
		assert("Invalid element type" && false);
		return "Invalid";
	}

	constexpr size_t VertexLayout::Element::SizeOf(ElementType type) noexcpt
	{
		{
			switch (type)
			{
			case Position2D: return sizeof(Map<Position2D>::SysType);
			case Position3D: return sizeof(Map<Position3D>::SysType);
			case Texture2D: return sizeof(Map<Texture2D>::SysType);
			case Normal: return sizeof(Map<Normal>::SysType);
			case Tangent: return sizeof(Map<Tangent>::SysType);
			case Bitangent: return sizeof(Map<Bitangent>::SysType);
			case Float3Color: return sizeof(Map<Float3Color>::SysType);
			case Float4Color: return sizeof(Map<Float4Color>::SysType);
			case RGBAColor: return sizeof(Map<RGBAColor>::SysType);
			default: ;
				assert("Invalid element type" && false);
			}
			return 0u;
		}
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
