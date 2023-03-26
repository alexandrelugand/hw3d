#pragma once

using ElementType = Dvtx::VertexLayout::ElementType;

namespace Geometry
{
	class Sphere
	{
	public:
		static IndexedTriangleList Make(int latDiv, int longDiv)
		{
			const auto layout = Dvtx::VertexLayout{}.Append(ElementType::Position3D);

			assert(latDiv >= 3);
			assert(longDiv >= 3);

			constexpr float radius = 1.0f;
			const auto base = XMVectorSet(0.0f, 0.0f, radius, 0.0f);
			const float lattitudeAngle = PI / static_cast<float>(latDiv);
			const float longitudeAngle = 2.0f * PI / static_cast<float>(longDiv);

			Dvtx::VertexBufferDescriptor vbd{std::move(layout)};
			for (int iLat = 1; iLat < latDiv; iLat++)
			{
				const auto latBase = XMVector3Transform(
					base,
					XMMatrixRotationX(lattitudeAngle * static_cast<float>(iLat))
				);
				for (int iLong = 0; iLong < longDiv; iLong++)
				{
					XMFLOAT3 calculatedPos{};
					auto v = XMVector3Transform(
						latBase,
						XMMatrixRotationZ(longitudeAngle * static_cast<float>(iLong))
					);
					XMStoreFloat3(&calculatedPos, v);
					vbd.EmplaceBack(calculatedPos);
				}
			}

			// add the cap vertices
			const auto iNorthPole = static_cast<unsigned short>(vbd.Size());
			XMFLOAT3 northPos{};
			XMStoreFloat3(&northPos, base);
			vbd.EmplaceBack(northPos);

			const auto iSouthPole = static_cast<unsigned short>(vbd.Size());
			XMFLOAT3 southPos{};
			XMStoreFloat3(&southPos, XMVectorNegate(base));
			vbd.EmplaceBack(southPos);

			const auto calcIdx = [longDiv](int iLat, int iLong)
			{
				return static_cast<unsigned short>(iLat * longDiv + iLong);
			};

			std::vector<unsigned short> indices;
			for (int iLat = 0; iLat < latDiv - 2; iLat++)
			{
				for (int iLong = 0; iLong < longDiv - 1; iLong++)
				{
					indices.push_back(calcIdx(iLat, iLong));
					indices.push_back(calcIdx(iLat + 1, iLong));
					indices.push_back(calcIdx(iLat, iLong + 1));
					indices.push_back(calcIdx(iLat, iLong + 1));
					indices.push_back(calcIdx(iLat + 1, iLong));
					indices.push_back(calcIdx(iLat + 1, iLong + 1));
				}
				// wrap band
				indices.push_back(calcIdx(iLat, longDiv - 1));
				indices.push_back(calcIdx(iLat + 1, longDiv - 1));
				indices.push_back(calcIdx(iLat, 0));
				indices.push_back(calcIdx(iLat, 0));
				indices.push_back(calcIdx(iLat + 1, longDiv - 1));
				indices.push_back(calcIdx(iLat + 1, 0));
			}

			// cap fans
			for (int iLong = 0; iLong < longDiv - 1; iLong++)
			{
				// north
				indices.push_back(iNorthPole);
				indices.push_back(calcIdx(0, iLong));
				indices.push_back(calcIdx(0, iLong + 1));
				// south
				indices.push_back(calcIdx(latDiv - 2, iLong + 1));
				indices.push_back(calcIdx(latDiv - 2, iLong));
				indices.push_back(iSouthPole);
			}
			// wrap triangles
			// north
			indices.push_back(iNorthPole);
			indices.push_back(calcIdx(0, longDiv - 1));
			indices.push_back(calcIdx(0, 0));
			// south
			indices.push_back(calcIdx(latDiv - 2, 0));
			indices.push_back(calcIdx(latDiv - 2, longDiv - 1));
			indices.push_back(iSouthPole);

			return {std::move(vbd), std::move(indices)};
		}
	};
}
