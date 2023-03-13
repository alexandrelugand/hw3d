#pragma once

struct RGBAColor
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
};

class VertexDescriptor
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
		using SysType = ::RGBAColor;
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

	VertexDescriptor& Append(ElementType type) noexcpt
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

class Vertex
{
	friend class VertexBufferDescriptor;

public:
	template <VertexDescriptor::ElementType Type>
	auto& Attr() noexcpt
	{
		auto pAttribute = pData + descriptor.Resolve<Type>().GetOffset();
		return *reinterpret_cast<typename VertexDescriptor::Map<Type>::SysType*>(pAttribute); //Magic of metadata template!
	}

	template <typename T>
	void SetAttributeByIndex(size_t i, T&& val) noexcpt
	{
		const auto& element = descriptor.ResolveByIndex(i);
		auto pAttribute = pData + element.GetOffset();
		switch (element.Type())
		{
		case VertexDescriptor::Position2D:
			SetAttribute<VertexDescriptor::Position2D>(pAttribute, std::forward<T>(val));
			break;
		case VertexDescriptor::Position3D:
			SetAttribute<VertexDescriptor::Position3D>(pAttribute, std::forward<T>(val));
			break;
		case VertexDescriptor::Texture2D:
			SetAttribute<VertexDescriptor::Texture2D>(pAttribute, std::forward<T>(val));
			break;
		case VertexDescriptor::Normal:
			SetAttribute<VertexDescriptor::Normal>(pAttribute, std::forward<T>(val));
			break;
		case VertexDescriptor::Float3Color:
			SetAttribute<VertexDescriptor::Float3Color>(pAttribute, std::forward<T>(val));
			break;
		case VertexDescriptor::Float4Color:
			SetAttribute<VertexDescriptor::Float4Color>(pAttribute, std::forward<T>(val));
			break;
		case VertexDescriptor::RGBAColor:
			SetAttribute<VertexDescriptor::RGBAColor>(pAttribute, std::forward<T>(val));
			break;
		default:
			assert("Bad element type" && false);
		}
	}

protected:
	Vertex(char* pData, const VertexDescriptor& descriptor)
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
	template <VertexDescriptor::ElementType DestElemType, typename Src>
	void SetAttribute(char* pAttribute, Src&& val) noexcpt
	{
		using Dest = typename VertexDescriptor::Map<DestElemType>::SysType;
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
	const VertexDescriptor& descriptor;
};

class ConstVertex
{
public:
	ConstVertex(const Vertex& vertex) noexcpt
		: vertex(vertex)
	{
	}

	template <VertexDescriptor::ElementType Type>
	const auto& Attr() const noexcpt
	{
		return const_cast<Vertex&>(vertex).Attr<Type>();
	}

private:
	Vertex vertex;
};

class VertexBufferDescriptor
{
public:
	VertexBufferDescriptor(VertexDescriptor descriptor) noexcpt
		: descriptor(descriptor)
	{
	}

	const VertexDescriptor& GetDescriptor() const noexcept
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
	VertexDescriptor descriptor;
};
