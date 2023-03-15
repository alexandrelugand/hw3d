#pragma once
#include "Color.h"

namespace Dvtx
{
	class VertexLayout
	{
	public:
		enum ElementType
		{
			Position2D,
			Position3D,
			Texture2D,
			Normal,
			Float3Color,
			Float4Color,
			RGBAColor,
		};

		//Metadata templates lookup map
		template <ElementType>
		struct Map;

		template <>
		struct Map<Position2D>
		{
			using SysType = XMFLOAT2;
			static constexpr DXGI_FORMAT dxgi_format = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "POSITION";
		};

		template <>
		struct Map<Position3D>
		{
			using SysType = XMFLOAT3;
			static constexpr DXGI_FORMAT dxgi_format = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "POSITION";
		};

		template <>
		struct Map<Texture2D>
		{
			using SysType = XMFLOAT2;
			static constexpr DXGI_FORMAT dxgi_format = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "TEXCOORD";
		};

		template <>
		struct Map<Normal>
		{
			using SysType = XMFLOAT3;
			static constexpr DXGI_FORMAT dxgi_format = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "NORMAL";
		};

		template <>
		struct Map<Float3Color>
		{
			using SysType = XMFLOAT3;
			static constexpr DXGI_FORMAT dxgi_format = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "COLOR";
		};

		template <>
		struct Map<Float4Color>
		{
			using SysType = XMFLOAT4;
			static constexpr DXGI_FORMAT dxgi_format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char* semantic = "COLOR";
		};

		template <>
		struct Map<RGBAColor>
		{
			using SysType = Dvtx::RGBAColor;
			static constexpr DXGI_FORMAT dxgi_format = DXGI_FORMAT_R8G8B8A8_UNORM;
			static constexpr const char* semantic = "COLOR";
		};

		class Element
		{
		public:
			Element(ElementType type, size_t offset)
				: type(type), offset(offset)
			{
			}

			size_t GetOffsetAfter() const noexcpt
			{
				return offset + Size();
			}

			size_t GetOffset() const
			{
				return offset;
			}

			size_t Size() const noexcpt
			{
				return SizeOf(type);
			}

			ElementType Type() const noexcpt
			{
				return type;
			}

			static constexpr size_t SizeOf(ElementType type) noexcpt
			{
				switch (type)
				{
				case Position2D: return sizeof(Map<Position2D>::SysType);
				case Position3D: return sizeof(Map<Position3D>::SysType);
				case Texture2D: return sizeof(Map<Texture2D>::SysType);
				case Normal: return sizeof(Map<Normal>::SysType);
				case Float3Color: return sizeof(Map<Float3Color>::SysType);
				case Float4Color: return sizeof(Map<Float4Color>::SysType);
				case RGBAColor: return sizeof(Map<RGBAColor>::SysType);
				default: ;
					assert("Invalid element type" && false);
				}
				return 0u;
			}

			D3D11_INPUT_ELEMENT_DESC GetDesc() const noexcpt
			{
				switch (type)
				{
				case Position2D: return GenerateDesc<Position2D>(GetOffset());
				case Position3D: return GenerateDesc<Position3D>(GetOffset());
				case Texture2D: return GenerateDesc<Texture2D>(GetOffset());
				case Normal: return GenerateDesc<Normal>(GetOffset());
				case Float3Color: return GenerateDesc<Float3Color>(GetOffset());
				case Float4Color: return GenerateDesc<Float4Color>(GetOffset());
				case RGBAColor: return GenerateDesc<RGBAColor>(GetOffset());
				default: ;
					assert("Invalid element type" && false);
				}
				return {"INVALID", 0, DXGI_FORMAT_UNKNOWN, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0};
			}

		private:
			ElementType type;
			size_t offset;

			template <ElementType type>
			static constexpr D3D11_INPUT_ELEMENT_DESC GenerateDesc(size_t offset) noexcpt
			{
				return {Map<type>::semantic, 0, Map<type>::dxgi_format, 0, static_cast<UINT>(offset), D3D11_INPUT_PER_VERTEX_DATA, 0};
			}
		};

		template <ElementType Type>
		const Element& Resolve() const noexcpt
		{
			for (auto& e : elements)
			{
				if (e.Type() == Type)
				{
					return e;
				}
			}
			assert("Could not resolve element type" && false);
			return elements.front();
		}

		const Element& ResolveByIndex(size_t index) const noexcpt
		{
			return elements[index];
		}

		VertexLayout& Append(ElementType type) noexcpt
		{
			elements.emplace_back(type, Size());
			return *this;
		}

		size_t Size() const noexcpt
		{
			return elements.empty() ? 0u : elements.back().GetOffsetAfter();
		}

		size_t GetElementCount() const noexcept
		{
			return elements.size();
		}

		std::vector<D3D11_INPUT_ELEMENT_DESC> GetLayout() const noexcpt
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> desc;
			desc.reserve(GetElementCount());
			for (const auto& e : elements)
			{
				desc.push_back(e.GetDesc());
			}
			return desc;
		}

	private:
		std::vector<Element> elements;
	};
}
