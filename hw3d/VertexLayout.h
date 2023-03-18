#pragma once
#include "Color.h"
// ReSharper disable CppExplicitSpecializationInNonNamespaceScope

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
			Tangent,
			Bitangent,
			Float3Color,
			Float4Color,
			RGBAColor
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
			static constexpr const char* code = "P2";
		};

		template <>
		struct Map<Position3D>
		{
			using SysType = XMFLOAT3;
			static constexpr DXGI_FORMAT dxgi_format = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "POSITION";
			static constexpr const char* code = "P3";
		};

		template <>
		struct Map<Texture2D>
		{
			using SysType = XMFLOAT2;
			static constexpr DXGI_FORMAT dxgi_format = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "TEXCOORD";
			static constexpr const char* code = "T2";
		};

		template <>
		struct Map<Normal>
		{
			using SysType = XMFLOAT3;
			static constexpr DXGI_FORMAT dxgi_format = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "NORMAL";
			static constexpr const char* code = "N";
		};

		template <>
		struct Map<Tangent>
		{
			using SysType = XMFLOAT3;
			static constexpr DXGI_FORMAT dxgi_format = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "TANGENT";
			static constexpr const char* code = "Nt";
		};

		template <>
		struct Map<Bitangent>
		{
			using SysType = XMFLOAT3;
			static constexpr DXGI_FORMAT dxgi_format = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "BITANGENT";
			static constexpr const char* code = "Nb";
		};

		template <>
		struct Map<Float3Color>
		{
			using SysType = XMFLOAT3;
			static constexpr DXGI_FORMAT dxgi_format = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "COLOR";
			static constexpr const char* code = "C3";
		};

		template <>
		struct Map<Float4Color>
		{
			using SysType = XMFLOAT4;
			static constexpr DXGI_FORMAT dxgi_format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char* semantic = "COLOR";
			static constexpr const char* code = "C4";
		};

		template <>
		struct Map<RGBAColor>
		{
			using SysType = Dvtx::RGBAColor;
			static constexpr DXGI_FORMAT dxgi_format = DXGI_FORMAT_R8G8B8A8_UNORM;
			static constexpr const char* semantic = "COLOR";
			static constexpr const char* code = "C8";
		};

		class Element
		{
		public:
			Element(ElementType type, size_t offset);

			size_t GetOffsetAfter() const noexcpt;
			size_t GetOffset() const;
			size_t Size() const noexcpt;
			ElementType Type() const noexcpt;
			static constexpr size_t SizeOf(ElementType type) noexcpt;
			D3D11_INPUT_ELEMENT_DESC GetDesc() const noexcpt;
			const char* GetCode() const noexcept;

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

		const Element& ResolveByIndex(size_t index) const noexcpt;
		VertexLayout& Append(ElementType type) noexcpt;
		size_t Size() const noexcpt;
		size_t GetElementCount() const noexcept;
		std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const noexcpt;
		std::string GetCode() const noexcpt;

	private:
		std::vector<Element> elements{};
	};
}
