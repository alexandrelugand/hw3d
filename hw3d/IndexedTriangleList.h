#pragma once

using ElementType = Dvtx::VertexLayout::ElementType;

namespace Geometry
{
	class IndexedTriangleList
	{
	public:
		IndexedTriangleList(Dvtx::VertexBufferDescriptor vbd_in, std::vector<unsigned short> indices_in)
			: vertices(std::move(vbd_in)), indices(std::move(indices_in))
		{
			assert(vertices.Size() > 2);
			assert(indices.size() % 3 == 0);
		}

		void Transform(FXMMATRIX matrix)
		{
			for (int i = 0; i < vertices.Size(); i++)
			{
				auto& pos = vertices[i].Attr<ElementType::Position3D>();
				XMStoreFloat3(
					&pos,
					XMVector3Transform(XMLoadFloat3(&pos), matrix)
				);
			}
		}

		void SetNormalsIndependentFlat() noexcpt
		{
			using namespace DirectX;
			using Type = Dvtx::VertexLayout::ElementType;
			for (size_t i = 0; i < indices.size(); i += 3)
			{
				auto v0 = vertices[indices[i]];
				auto v1 = vertices[indices[i + 1]];
				auto v2 = vertices[indices[i + 2]];
				const auto p0 = XMLoadFloat3(&v0.Attr<Type::Position3D>());
				const auto p1 = XMLoadFloat3(&v1.Attr<Type::Position3D>());
				const auto p2 = XMLoadFloat3(&v2.Attr<Type::Position3D>());

				const auto n = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));

				XMStoreFloat3(&v0.Attr<Type::Normal>(), n);
				XMStoreFloat3(&v1.Attr<Type::Normal>(), n);
				XMStoreFloat3(&v2.Attr<Type::Normal>(), n);
			}
		}

		void SetNormalsAndTBNIndependentFlat() noexcpt
		{
			using namespace DirectX;
			using Type = Dvtx::VertexLayout::ElementType;
			for (size_t i = 0; i < indices.size(); i += 3)
			{
				auto v0 = vertices[indices[i]];
				auto v1 = vertices[indices[i + 1]];
				auto v2 = vertices[indices[i + 2]];
				const auto p0 = XMLoadFloat3(&v0.Attr<Type::Position3D>());
				const auto p1 = XMLoadFloat3(&v1.Attr<Type::Position3D>());
				const auto p2 = XMLoadFloat3(&v2.Attr<Type::Position3D>());
				const auto t0 = XMLoadFloat2(&v0.Attr<Type::Texture2D>());
				const auto t1 = XMLoadFloat2(&v1.Attr<Type::Texture2D>());
				const auto t2 = XMLoadFloat2(&v2.Attr<Type::Texture2D>());

				const auto n = -XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));

				XMStoreFloat3(&v0.Attr<Type::Normal>(), n);
				XMStoreFloat3(&v1.Attr<Type::Normal>(), n);
				XMStoreFloat3(&v2.Attr<Type::Normal>(), n);

				XMVECTOR tangent;
				XMVECTOR binormal;
				ComputeTangents(p0, p1, p2, t0, t1, t2, tangent, binormal);

				XMStoreFloat3(&v0.Attr<Type::Tangent>(), tangent);
				XMStoreFloat3(&v1.Attr<Type::Tangent>(), tangent);
				XMStoreFloat3(&v2.Attr<Type::Tangent>(), tangent);

				XMStoreFloat3(&v0.Attr<Type::Bitangent>(), binormal);
				XMStoreFloat3(&v1.Attr<Type::Bitangent>(), binormal);
				XMStoreFloat3(&v2.Attr<Type::Bitangent>(), binormal);
			}
		}

		void ComputeTangents(const XMVECTOR& v0, const XMVECTOR& v1, const XMVECTOR& v2, const XMVECTOR& t0, const XMVECTOR& t1, const XMVECTOR& t2, XMVECTOR& tangent, XMVECTOR& binormal)
		{
			const XMVECTOR deltaPos1 = XMVectorSubtract(v1, v0);
			const XMVECTOR deltaPos2 = XMVectorSubtract(v2, v0);

			const XMVECTOR deltaUV1 = XMVectorSubtract(t1, t0);
			const XMVECTOR deltaUV2 = XMVectorSubtract(t2, t0);

			const float r = 1.0f / (deltaUV1.m128_f32[0] * deltaUV2.m128_f32[1] - deltaUV1.m128_f32[1] * deltaUV2.m128_f32[0]);
			tangent = (deltaPos1 * deltaUV2.m128_f32[1] - deltaPos2 * deltaUV1.m128_f32[1]);
			tangent = tangent * r;
			tangent = XMVector3Normalize(tangent);
			binormal = (deltaPos2 * deltaUV1.m128_f32[0] - deltaPos1 * deltaUV2.m128_f32[0]);
			binormal = binormal * r;
			XMVector3Normalize(binormal);
		}

		// asserts face-independent vertices w/ normals cleared to zero
		static void SetNormalsIndependentFlat(const std::vector<XMFLOAT3>& vertices, const std::vector<unsigned short>& indices, std::vector<XMFLOAT3>& normals) noexcpt
		{
			using ElementType = Dvtx::VertexLayout::ElementType;
			assert(indices.size() % 3 == 0 && indices.size() > 0);
			for (size_t i = 0; i < indices.size(); i += 3)
			{
				auto v0 = vertices[indices[i]];
				auto v1 = vertices[indices[i + 1]];
				auto v2 = vertices[indices[i + 2]];
				const auto p0 = XMLoadFloat3(&v0);
				const auto p1 = XMLoadFloat3(&v1);
				const auto p2 = XMLoadFloat3(&v2);

				const auto n = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));

				XMFLOAT3 n0{}, n1{}, n2{};
				XMStoreFloat3(&n0, n);
				normals.push_back(n0);
				XMStoreFloat3(&n1, n);
				normals.push_back(n1);
				XMStoreFloat3(&n2, n);
				normals.push_back(n2);
			}
		}

		Dvtx::VertexBufferDescriptor vertices;
		std::vector<unsigned short> indices{};
	};
}
