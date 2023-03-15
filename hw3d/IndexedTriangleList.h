#pragma once

namespace Geometry
{
	template <class T>
	class IndexedTriangleList
	{
	public:
		IndexedTriangleList() = default;

		IndexedTriangleList(std::vector<T> verts_in, std::vector<unsigned short> indices_in)
			: vertices(std::move(verts_in)), indices(std::move(indices_in))
		{
			assert(vertices.size() > 2);
			assert(indices.size() % 3 == 0);
		}

		void Transform(FXMMATRIX matrix)
		{
			for (auto& v : vertices)
			{
				const XMVECTOR pos = XMLoadFloat3(&v.pos);
				XMStoreFloat3(
					&v.pos,
					XMVector3Transform(pos, matrix)
				);
			}
		}

		// asserts face-independent vertices w/ normals cleared to zero
		void SetNormalsIndependentFlat() noexcpt
		{
			assert(indices.size() % 3 == 0 && indices.size() > 0);
			for (size_t i = 0; i < indices.size(); i += 3)
			{
				auto& v0 = vertices[indices[i]];
				auto& v1 = vertices[indices[i + 1]];
				auto& v2 = vertices[indices[i + 2]];
				const auto p0 = XMLoadFloat3(&v0.pos);
				const auto p1 = XMLoadFloat3(&v1.pos);
				const auto p2 = XMLoadFloat3(&v2.pos);

				const auto n = XMVector3Normalize(XMVector3Cross((p1 - p0), (p2 - p0)));

				XMStoreFloat3(&v0.n, n);
				XMStoreFloat3(&v1.n, n);
				XMStoreFloat3(&v2.n, n);
			}
		}

	public:
		std::vector<T> vertices;
		std::vector<unsigned short> indices;
	};
}
