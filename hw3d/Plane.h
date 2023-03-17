#pragma once

using ElementType = Dvtx::VertexLayout::ElementType;

namespace Geometry
{
	class Plane
	{
	public:
		static IndexedTriangleList Make(int divisions_x, int divisions_y)
		{
			assert(divisions_x >= 1);
			assert(divisions_y >= 1);

			const auto layout = Dvtx::VertexLayout{}
			                    .Append(ElementType::Position3D)
			                    .Append(ElementType::Normal)
			                    .Append(ElementType::Texture2D);

			constexpr float width = 2.0f;
			constexpr float height = 2.0f;
			const int nVertices_x = divisions_x + 1;
			const int nVertices_y = divisions_y + 1;
			Dvtx::VertexBufferDescriptor vbd{std::move(layout)};

			{
				const float side_x = width / 2.0f;
				const float side_y = height / 2.0f;
				const float divisionSize_x = width / static_cast<float>(divisions_x);
				const float divisionSize_y = height / static_cast<float>(divisions_y);
				const float divisionSize_x_tc = 1.0f / static_cast<float>(divisions_x);
				const float divisionSize_y_tc = 1.0f / static_cast<float>(divisions_y);

				for (int y = 0, i = 0; y < nVertices_y; y++)
				{
					const float y_pos = static_cast<float>(y) * divisionSize_y - 1.0f;
					const float y_pos_tc = 1.0f - static_cast<float>(y) * divisionSize_y_tc;
					for (int x = 0; x < nVertices_x; x++, i++)
					{
						const float x_pos = static_cast<float>(x) * divisionSize_x - 1.0f;
						const float x_pos_tc = static_cast<float>(x) * divisionSize_x_tc;
						vbd.EmplaceBack(
							XMFLOAT3{x_pos, y_pos, 0.0f},
							XMFLOAT3{0.0f, 0.0f, -1.0f},
							XMFLOAT2{x_pos_tc, y_pos_tc}
						);
					}
				}
			}

			std::vector<unsigned short> indices;
			indices.reserve(sq(divisions_x * divisions_y) * 6);
			{
				const auto vxy2i = [nVertices_x](size_t x, size_t y)
				{
					return static_cast<unsigned short>(y * nVertices_x + x);
				};

				for (size_t y = 0; y < divisions_y; y++)
				{
					for (size_t x = 0; x < divisions_x; x++)
					{
						const std::array<unsigned short, 4> indexArray =
							{vxy2i(x, y), vxy2i(x + 1, y), vxy2i(x, y + 1), vxy2i(x + 1, y + 1)};
						indices.push_back(indexArray[0]);
						indices.push_back(indexArray[2]);
						indices.push_back(indexArray[1]);
						indices.push_back(indexArray[1]);
						indices.push_back(indexArray[2]);
						indices.push_back(indexArray[3]);
					}
				}
			}

			return {std::move(vbd), std::move(indices)};
		}
	};
}
