#pragma once
#include "Color.h"
// ReSharper disable CppExplicitSpecializationInNonNamespaceScope

#define DVTX_ELEMENT_AI_EXTRACTOR(member) static SysType Extract( const aiMesh& mesh,size_t i ) noexcept {return *reinterpret_cast<const SysType*>(&mesh.member[i]);}

#define LAYOUT_ELEMENT_TYPES \
	X( Position2D ) \
	X( Position3D ) \
	X( Texture2D ) \
	X( Normal ) \
	X( Tangent ) \
	X( Bitangent ) \
	X( Float3Color ) \
	X( Float4Color ) \
	X( RGBAColor ) \
	X( Count )

namespace Dvtx
{
	class VertexLayout
	{
	public:
		enum ElementType
		{
#define X(el) el,
			LAYOUT_ELEMENT_TYPES
#undef X
		};

		//Metadata templates lookup map
		template <ElementType>
		struct Map;

		template <>
		struct Map<Position2D>
		{
			using SysType = XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "POSITION";
			static constexpr const char* code = "P2";
			DVTX_ELEMENT_AI_EXTRACTOR(mVertices)
		};

		template <>
		struct Map<Position3D>
		{
			using SysType = XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "POSITION";
			static constexpr const char* code = "P3";
			DVTX_ELEMENT_AI_EXTRACTOR(mVertices)
		};

		template <>
		struct Map<Texture2D>
		{
			using SysType = XMFLOAT2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "TEXCOORD";
			static constexpr const char* code = "T2";
			DVTX_ELEMENT_AI_EXTRACTOR(mTextureCoords[0])
		};

		template <>
		struct Map<Normal>
		{
			using SysType = XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "NORMAL";
			static constexpr const char* code = "N";
			DVTX_ELEMENT_AI_EXTRACTOR(mNormals)
		};

		template <>
		struct Map<Tangent>
		{
			using SysType = XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "TANGENT";
			static constexpr const char* code = "Nt";
			DVTX_ELEMENT_AI_EXTRACTOR(mTangents)
		};

		template <>
		struct Map<Bitangent>
		{
			using SysType = XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "BITANGENT";
			static constexpr const char* code = "Nb";
			DVTX_ELEMENT_AI_EXTRACTOR(mBitangents)
		};

		template <>
		struct Map<Float3Color>
		{
			using SysType = XMFLOAT3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "COLOR";
			static constexpr const char* code = "C3";
			DVTX_ELEMENT_AI_EXTRACTOR(mColors[0])
		};

		template <>
		struct Map<Float4Color>
		{
			using SysType = XMFLOAT4;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char* semantic = "COLOR";
			static constexpr const char* code = "C4";
			DVTX_ELEMENT_AI_EXTRACTOR(mColors[0])
		};

		template <>
		struct Map<RGBAColor>
		{
			using SysType = Dvtx::RGBAColor;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			static constexpr const char* semantic = "COLOR";
			static constexpr const char* code = "C8";
			DVTX_ELEMENT_AI_EXTRACTOR(mColors[0])
		};

		template <>
		struct Map<Count>
		{
			using SysType = long double;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_UNKNOWN;
			static constexpr const char* semantic = "!INVALID!";
			static constexpr const char* code = "!INV!";
			DVTX_ELEMENT_AI_EXTRACTOR(mFaces)
		};

		template <template<ElementType> class F, typename... Args>
		static constexpr auto Bridge(ElementType type, Args&&... args) noexcpt
		{
			switch (type)
			{
#define X(el) case VertexLayout::el: return F<VertexLayout::el>::Exec(std::forward<Args>(args)...);
			LAYOUT_ELEMENT_TYPES
#undef X
			}
			assert("Invalid element type" && false);
			return F<Count>::Exec(std::forward<Args>(args)...);
		}

		class Element
		{
		public:
			Element(ElementType type, size_t offset);

			size_t GetOffsetAfter() const noexcpt;
			size_t GetOffset() const;
			size_t Size() const noexcpt;
			static constexpr size_t SizeOf(ElementType type) noexcpt;

			template <ElementType type>
			struct SysSizeLookup
			{
				static constexpr auto Exec() noexcept
				{
					return sizeof(Map<type>::SysType);
				}
			};

			template <ElementType type>
			struct DescGenerate
			{
				static constexpr D3D11_INPUT_ELEMENT_DESC Exec(size_t offset) noexcept
				{
					return {
						Map<type>::semantic,
						0,
						Map<type>::dxgiFormat,
						0,
						static_cast<UINT>(offset),
						D3D11_INPUT_PER_VERTEX_DATA,
						0
					};
				}
			};

			template <ElementType type>
			struct CodeLookup
			{
				static constexpr auto Exec() noexcept
				{
					return Map<type>::code;
				}
			};

			ElementType GetType() const noexcept;
			D3D11_INPUT_ELEMENT_DESC GetDesc() const noexcpt;
			const char* GetCode() const noexcept;

		private:
			ElementType type;
			size_t offset;
		};

		template <ElementType Type>
		const Element& Resolve() const noexcpt
		{
			for (auto& e : elements)
			{
				if (e.GetType() == Type)
				{
					return e;
				}
			}
			assert("Could not resolve element type" && false);
			return elements.front();
		}

		const Element& ResolveByIndex(size_t index) const noexcpt;
		VertexLayout& Append(ElementType type) noexcpt;
		bool Has(ElementType type) const noexcept;
		size_t Size() const noexcpt;
		size_t GetElementCount() const noexcept;
		std::vector<D3D11_INPUT_ELEMENT_DESC> GetD3DLayout() const noexcpt;
		std::string GetCode() const noexcpt;

	private:
		std::vector<Element> elements{};
	};
}
