#pragma once
#include "VertexBufferDescriptor.h"

using ElementType = Dvtx::VertexLayout::ElementType;

namespace Geometry
{
	class Cube
	{
	public:
		static IndexedTriangleList Make()
		{
			return MakeIndependent(std::move(Dvtx::VertexLayout{}
			                                 .Append(ElementType::Position3D)
			                                 .Append(ElementType::Normal)
			));
		}

		static IndexedTriangleList MakeIndependent(Dvtx::VertexLayout layout)
		{
			constexpr float side = 1.0f / 2.0f;

			Dvtx::VertexBufferDescriptor vbd(std::move(layout), 24u);
			vbd[0].Attr<ElementType::Position3D>() = {-side, -side, -side}; // 0 near side
			vbd[1].Attr<ElementType::Position3D>() = {side, -side, -side}; // 1
			vbd[2].Attr<ElementType::Position3D>() = {-side, side, -side}; // 2
			vbd[3].Attr<ElementType::Position3D>() = {side, side, -side}; // 3
			vbd[4].Attr<ElementType::Position3D>() = {-side, -side, side}; // 4 far side
			vbd[5].Attr<ElementType::Position3D>() = {side, -side, side}; // 5
			vbd[6].Attr<ElementType::Position3D>() = {-side, side, side}; // 6
			vbd[7].Attr<ElementType::Position3D>() = {side, side, side}; // 7
			vbd[8].Attr<ElementType::Position3D>() = {-side, -side, -side}; // 8 left side
			vbd[9].Attr<ElementType::Position3D>() = {-side, side, -side}; // 9
			vbd[10].Attr<ElementType::Position3D>() = {-side, -side, side}; // 10
			vbd[11].Attr<ElementType::Position3D>() = {-side, side, side}; // 11
			vbd[12].Attr<ElementType::Position3D>() = {side, -side, -side}; // 12 right side
			vbd[13].Attr<ElementType::Position3D>() = {side, side, -side}; // 13
			vbd[14].Attr<ElementType::Position3D>() = {side, -side, side}; // 14
			vbd[15].Attr<ElementType::Position3D>() = {side, side, side}; // 15
			vbd[16].Attr<ElementType::Position3D>() = {-side, -side, -side}; // 16 bottom side
			vbd[17].Attr<ElementType::Position3D>() = {side, -side, -side}; // 17
			vbd[18].Attr<ElementType::Position3D>() = {-side, -side, side}; // 18
			vbd[19].Attr<ElementType::Position3D>() = {side, -side, side}; // 19
			vbd[20].Attr<ElementType::Position3D>() = {-side, side, -side}; // 20 top side
			vbd[21].Attr<ElementType::Position3D>() = {side, side, -side}; // 21
			vbd[22].Attr<ElementType::Position3D>() = {-side, side, side}; // 22
			vbd[23].Attr<ElementType::Position3D>() = {side, side, side}; // 23

			return {
				std::move(vbd), {
					0, 2, 1, 2, 3, 1,
					4, 5, 7, 4, 7, 6,
					8, 10, 9, 10, 11, 9,
					12, 13, 15, 12, 15, 14,
					16, 17, 18, 18, 17, 19,
					20, 23, 21, 20, 22, 23
				}
			};
		}

		static IndexedTriangleList MakeIndependentTextured()
		{
			auto itl = MakeIndependent(std::move(Dvtx::VertexLayout{}
			                                     .Append(ElementType::Position3D)
			                                     .Append(ElementType::Normal)
			                                     .Append(ElementType::Texture2D)
			));

			itl.vertices[0].Attr<ElementType::Texture2D>() = {0.0f, 0.0f};
			itl.vertices[1].Attr<ElementType::Texture2D>() = {1.0f, 0.0f};
			itl.vertices[2].Attr<ElementType::Texture2D>() = {0.0f, 1.0f};
			itl.vertices[3].Attr<ElementType::Texture2D>() = {1.0f, 1.0f};
			itl.vertices[4].Attr<ElementType::Texture2D>() = {0.0f, 0.0f};
			itl.vertices[5].Attr<ElementType::Texture2D>() = {1.0f, 0.0f};
			itl.vertices[6].Attr<ElementType::Texture2D>() = {0.0f, 1.0f};
			itl.vertices[7].Attr<ElementType::Texture2D>() = {1.0f, 1.0f};
			itl.vertices[8].Attr<ElementType::Texture2D>() = {0.0f, 0.0f};
			itl.vertices[9].Attr<ElementType::Texture2D>() = {1.0f, 0.0f};
			itl.vertices[10].Attr<ElementType::Texture2D>() = {0.0f, 1.0f};
			itl.vertices[11].Attr<ElementType::Texture2D>() = {1.0f, 1.0f};
			itl.vertices[12].Attr<ElementType::Texture2D>() = {0.0f, 0.0f};
			itl.vertices[13].Attr<ElementType::Texture2D>() = {1.0f, 0.0f};
			itl.vertices[14].Attr<ElementType::Texture2D>() = {0.0f, 1.0f};
			itl.vertices[15].Attr<ElementType::Texture2D>() = {1.0f, 1.0f};
			itl.vertices[16].Attr<ElementType::Texture2D>() = {0.0f, 0.0f};
			itl.vertices[17].Attr<ElementType::Texture2D>() = {1.0f, 0.0f};
			itl.vertices[18].Attr<ElementType::Texture2D>() = {0.0f, 1.0f};
			itl.vertices[19].Attr<ElementType::Texture2D>() = {1.0f, 1.0f};
			itl.vertices[20].Attr<ElementType::Texture2D>() = {0.0f, 0.0f};
			itl.vertices[21].Attr<ElementType::Texture2D>() = {1.0f, 0.0f};
			itl.vertices[22].Attr<ElementType::Texture2D>() = {0.0f, 1.0f};
			itl.vertices[23].Attr<ElementType::Texture2D>() = {1.0f, 1.0f};

			return itl;
		}
	};
}
