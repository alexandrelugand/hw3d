#pragma once

using ElementType = Dvtx::VertexLayout::ElementType;

namespace Geometry
{
	class IndexedTriangleList
	{
	public:
		IndexedTriangleList(Dvtx::VertexBufferDescriptor vbd_in, std::vector<unsigned short> indices_in)
			: vbd(std::move(vbd_in)), indices(std::move(indices_in))
		{
			assert(vbd.NumVertices() > 2);
			assert(indices.size() % 3 == 0);
		}

		void Transform(FXMMATRIX matrix)
		{
			for (int i = 0; i < vbd.NumVertices(); i++)
			{
				auto& pos = vbd[i].Attr<ElementType::Position3D>();
				XMStoreFloat3(
					&pos,
					XMVector3Transform(XMLoadFloat3(&pos), matrix)
				);
			}
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

		Dvtx::VertexBufferDescriptor vbd;
		std::vector<unsigned short> indices{};
	};
}
