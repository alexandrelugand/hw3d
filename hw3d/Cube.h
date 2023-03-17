#pragma once

using ElementType = Dvtx::VertexLayout::ElementType;

namespace Geometry
{
	class Cube
	{
	public:
		static IndexedTriangleList Make()
		{
			const auto layout = Dvtx::VertexLayout{}
				.Append(ElementType::Position3D);

			constexpr float side = 1.0f / 2.0f;

			Dvtx::VertexBufferDescriptor vbd{std::move(layout)};

			vbd.EmplaceBack(XMFLOAT3{-side, -side, -side});
			vbd.EmplaceBack(XMFLOAT3{side, -side, -side});
			vbd.EmplaceBack(XMFLOAT3{-side, side, -side});
			vbd.EmplaceBack(XMFLOAT3{side, side, -side});
			vbd.EmplaceBack(XMFLOAT3{-side, -side, side});
			vbd.EmplaceBack(XMFLOAT3{side, -side, side});
			vbd.EmplaceBack(XMFLOAT3{-side, side, side});
			vbd.EmplaceBack(XMFLOAT3{side, side, side});

			return {
				std::move(vbd), {
					0, 2, 1, 2, 3, 1,
					1, 3, 5, 3, 7, 5,
					2, 6, 3, 3, 6, 7,
					4, 5, 7, 4, 7, 6,
					0, 4, 2, 2, 4, 6,
					0, 1, 4, 1, 5, 4
				}
			};
		}

		static IndexedTriangleList MakeIndependent()
		{
			const auto layout = Dvtx::VertexLayout{}
			                    .Append(ElementType::Position3D)
			                    .Append(ElementType::Normal);

			constexpr float side = 1.0f / 2.0f;

			Dvtx::VertexBufferDescriptor vbd{std::move(layout)};

			std::vector<XMFLOAT3> vertices;
			vertices.reserve(24);
			vertices.push_back(XMFLOAT3{-side, -side, -side}); // 0 near side
			vertices.push_back(XMFLOAT3{side, -side, -side}); // 1
			vertices.push_back(XMFLOAT3{-side, side, -side}); // 2
			vertices.push_back(XMFLOAT3{side, side, -side}); // 3
			vertices.push_back(XMFLOAT3{-side, -side, side}); // 4 far side
			vertices.push_back(XMFLOAT3{side, -side, side}); // 5
			vertices.push_back(XMFLOAT3{-side, side, side}); // 6
			vertices.push_back(XMFLOAT3{side, side, side}); // 7
			vertices.push_back(XMFLOAT3{-side, -side, -side}); // 8 left side
			vertices.push_back(XMFLOAT3{-side, side, -side}); // 9
			vertices.push_back(XMFLOAT3{-side, -side, side}); // 10
			vertices.push_back(XMFLOAT3{-side, side, side}); // 11
			vertices.push_back(XMFLOAT3{side, -side, -side}); // 12 right side
			vertices.push_back(XMFLOAT3{side, side, -side}); // 13
			vertices.push_back(XMFLOAT3{side, -side, side}); // 14
			vertices.push_back(XMFLOAT3{side, side, side}); // 15
			vertices.push_back(XMFLOAT3{-side, -side, -side}); // 16 bottom side
			vertices.push_back(XMFLOAT3{side, -side, -side}); // 17
			vertices.push_back(XMFLOAT3{-side, -side, side}); // 18
			vertices.push_back(XMFLOAT3{side, -side, side}); // 19
			vertices.push_back(XMFLOAT3{-side, side, -side}); // 20 top side
			vertices.push_back(XMFLOAT3{side, side, -side}); // 21
			vertices.push_back(XMFLOAT3{-side, side, side}); // 22
			vertices.push_back(XMFLOAT3{side, side, side}); // 23

			std::vector<unsigned short> indices{
				0, 2, 1, 2, 3, 1,
				4, 5, 7, 4, 7, 6,
				8, 10, 9, 10, 11, 9,
				12, 13, 15, 12, 15, 14,
				16, 17, 18, 18, 17, 19,
				20, 23, 21, 20, 22, 23
			};

			std::vector<XMFLOAT3> normals;
			normals.reserve(24);
			IndexedTriangleList::SetNormalsIndependentFlat(vertices, indices, normals);

			for (unsigned int i = 0; i < vertices.size(); i++)
			{
				vbd.EmplaceBack(vertices[i], normals[i]);
			}

			return {
				std::move(vbd),
				std::move(indices)
			};
		}

		static IndexedTriangleList MakeIndependentTextured()
		{
			const auto layout = Dvtx::VertexLayout{}
			                    .Append(ElementType::Position3D)
			                    .Append(ElementType::Normal)
			                    .Append(ElementType::Texture2D);

			constexpr float side = 1.0f / 2.0f;

			Dvtx::VertexBufferDescriptor vbd{std::move(layout)};

			std::vector<XMFLOAT3> vertices;
			vertices.reserve(24);
			vertices.push_back(XMFLOAT3{-side, -side, -side}); // 0 near side
			vertices.push_back(XMFLOAT3{side, -side, -side}); // 1
			vertices.push_back(XMFLOAT3{-side, side, -side}); // 2
			vertices.push_back(XMFLOAT3{side, side, -side}); // 3
			vertices.push_back(XMFLOAT3{-side, -side, side}); // 4 far side
			vertices.push_back(XMFLOAT3{side, -side, side}); // 5
			vertices.push_back(XMFLOAT3{-side, side, side}); // 6
			vertices.push_back(XMFLOAT3{side, side, side}); // 7
			vertices.push_back(XMFLOAT3{-side, -side, -side}); // 8 left side
			vertices.push_back(XMFLOAT3{-side, side, -side}); // 9
			vertices.push_back(XMFLOAT3{-side, -side, side}); // 10
			vertices.push_back(XMFLOAT3{-side, side, side}); // 11
			vertices.push_back(XMFLOAT3{side, -side, -side}); // 12 right side
			vertices.push_back(XMFLOAT3{side, side, -side}); // 13
			vertices.push_back(XMFLOAT3{side, -side, side}); // 14
			vertices.push_back(XMFLOAT3{side, side, side}); // 15
			vertices.push_back(XMFLOAT3{-side, -side, -side}); // 16 bottom side
			vertices.push_back(XMFLOAT3{side, -side, -side}); // 17
			vertices.push_back(XMFLOAT3{-side, -side, side}); // 18
			vertices.push_back(XMFLOAT3{side, -side, side}); // 19
			vertices.push_back(XMFLOAT3{-side, side, -side}); // 20 top side
			vertices.push_back(XMFLOAT3{side, side, -side}); // 21
			vertices.push_back(XMFLOAT3{-side, side, side}); // 22
			vertices.push_back(XMFLOAT3{side, side, side}); // 23

			std::vector<unsigned short> indices{
				0, 2, 1, 2, 3, 1,
				4, 5, 7, 4, 7, 6,
				8, 10, 9, 10, 11, 9,
				12, 13, 15, 12, 15, 14,
				16, 17, 18, 18, 17, 19,
				20, 23, 21, 20, 22, 23
			};

			std::vector<XMFLOAT3> normals;
			normals.reserve(24);
			IndexedTriangleList::SetNormalsIndependentFlat(vertices, indices, normals);

			std::vector<XMFLOAT2> texcoords;
			texcoords.reserve(24);
			texcoords.push_back(XMFLOAT2{0.0f, 0.0f}); // 0 near side
			texcoords.push_back(XMFLOAT2{1.0f, 0.0f}); // 1
			texcoords.push_back(XMFLOAT2{0.0f, 1.0f}); // 2
			texcoords.push_back(XMFLOAT2{1.0f, 1.0f}); // 3
			texcoords.push_back(XMFLOAT2{0.0f, 0.0f}); // 4 far side
			texcoords.push_back(XMFLOAT2{1.0f, 0.0f}); // 5
			texcoords.push_back(XMFLOAT2{0.0f, 1.0f}); // 6
			texcoords.push_back(XMFLOAT2{1.0f, 1.0f}); // 7
			texcoords.push_back(XMFLOAT2{0.0f, 0.0f}); // 8 left side
			texcoords.push_back(XMFLOAT2{1.0f, 0.0f}); // 9
			texcoords.push_back(XMFLOAT2{0.0f, 1.0f}); // 10
			texcoords.push_back(XMFLOAT2{1.0f, 1.0f}); // 11
			texcoords.push_back(XMFLOAT2{0.0f, 0.0f}); // 12 right side
			texcoords.push_back(XMFLOAT2{1.0f, 0.0f}); // 13
			texcoords.push_back(XMFLOAT2{0.0f, 1.0f}); // 14
			texcoords.push_back(XMFLOAT2{1.0f, 1.0f}); // 15
			texcoords.push_back(XMFLOAT2{0.0f, 0.0f}); // 16 bottom side
			texcoords.push_back(XMFLOAT2{1.0f, 0.0f}); // 17
			texcoords.push_back(XMFLOAT2{0.0f, 1.0f}); // 18
			texcoords.push_back(XMFLOAT2{1.0f, 1.0f}); // 19
			texcoords.push_back(XMFLOAT2{0.0f, 0.0f}); // 20 top side
			texcoords.push_back(XMFLOAT2{1.0f, 0.0f}); // 21
			texcoords.push_back(XMFLOAT2{0.0f, 1.0f}); // 22
			texcoords.push_back(XMFLOAT2{1.0f, 1.0f}); // 23

			for (unsigned int i = 0; i < vertices.size(); i++)
			{
				vbd.EmplaceBack(vertices[i], normals[i], texcoords[i]);
			}

			return {
				std::move(vbd),
				std::move(indices)
			};
		}
	};
}
