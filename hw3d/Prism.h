#pragma once

using ElementType = Dvtx::VertexLayout::ElementType;

namespace Geometry
{
	class Prism
	{
	public:
		static IndexedTriangleList MakeTesselated(int longDiv)
		{
			assert(longDiv >= 3);

			const auto layout = Dvtx::VertexLayout{}.Append(ElementType::Position3D);

			const auto base = XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
			const auto offset = XMVectorSet(0.0f, 0.0f, 2.0f, 0.0f);
			const float longitudeAngle = 2.0f * PI / longDiv;

			// near center
			Dvtx::VertexBufferDescriptor vbd{std::move(layout)};
			std::vector<XMFLOAT3> vertices;

			XMFLOAT3 centerNear{0.0f, 0.0f, -1.0f};
			vertices.push_back(centerNear);
			const auto iCenterNear = static_cast<unsigned short>(vertices.size() - 1);

			// far center
			XMFLOAT3 centerFar{0.0f, 0.0f, 1.0f};
			vertices.push_back(centerFar);
			const auto iCenterFar = static_cast<unsigned short>(vertices.size() - 1);

			// base vertices
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				// near base
				{
					XMFLOAT3 nearBase{};
					auto v = XMVector3Transform(
						base,
						XMMatrixRotationZ(longitudeAngle * iLong)
					);
					XMStoreFloat3(&nearBase, v);
					vertices.push_back(nearBase);
				}

				// far base
				{
					XMFLOAT3 farBase{};
					auto v = XMVector3Transform(
						base,
						XMMatrixRotationZ(longitudeAngle * iLong)
					);
					v = XMVectorAdd(v, offset);
					XMStoreFloat3(&farBase, v);
					vertices.push_back(farBase);
				}
			}

			// side indices
			std::vector<unsigned short> indices;
			for (unsigned short iLong = 0; iLong < longDiv; iLong++)
			{
				const auto i = iLong * 2;
				const auto mod = longDiv * 2;
				indices.push_back(i + 2);
				indices.push_back((i + 2) % mod + 2);
				indices.push_back(i + 1 + 2);
				indices.push_back((i + 2) % mod + 2);
				indices.push_back((i + 3) % mod + 2);
				indices.push_back(i + 1 + 2);
			}

			// base indices
			for (unsigned short iLong = 0; iLong < longDiv; iLong++)
			{
				const auto i = iLong * 2;
				const auto mod = longDiv * 2;
				indices.push_back(i + 2);
				indices.push_back(iCenterNear);
				indices.push_back((i + 2) % mod + 2);
				indices.push_back(iCenterFar);
				indices.push_back(i + 1 + 2);
				indices.push_back((i + 3) % mod + 2);
			}

			for (unsigned int i = 0; i < vertices.size(); i++)
			{
				vbd.EmplaceBack(vertices[i]);
			}

			return {std::move(vbd), std::move(indices)};
		}

		static IndexedTriangleList MakeTesselatedIndependentNormals(int longDiv)
		{
			assert(longDiv >= 3);

			const auto layout = Dvtx::VertexLayout{}
			                    .Append(ElementType::Position3D)
			                    .Append(ElementType::Normal);

			const auto base = XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
			const auto offset = XMVectorSet(0.0f, 0.0f, 2.0f, 0.0f);
			const float longitudeAngle = 2.0f * PI / longDiv;

			Dvtx::VertexBufferDescriptor vbd{std::move(layout)};
			std::vector<XMFLOAT3> vertices;
			std::vector<XMFLOAT3> normals;

			// near center
			const auto iCenterNear = static_cast<unsigned short>(vertices.size());
			XMFLOAT3 centerNear{0.0f, 0.0f, -1.0f};
			vertices.push_back(centerNear);
			normals.push_back(centerNear);

			// near base vertices
			const auto iBaseNear = static_cast<unsigned short>(vertices.size());
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				XMFLOAT3 calculatedPos{};
				auto v = XMVector3Transform(
					base,
					XMMatrixRotationZ(longitudeAngle * iLong)
				);
				XMStoreFloat3(&calculatedPos, v);
				vertices.push_back(calculatedPos);
				normals.push_back(XMFLOAT3{0.0f, 0.0f, -1.0f});
			}

			// far center
			const auto iCenterFar = static_cast<unsigned short>(vertices.size());
			vertices.push_back(XMFLOAT3{0.0f, 0.0f, 1.0f});
			normals.push_back(XMFLOAT3{0.0f, 0.0f, 1.0f});

			// far base vertices
			const auto iBaseFar = static_cast<unsigned short>(vertices.size());
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				XMFLOAT3 calculatedPos;
				auto v = XMVector3Transform(
					base,
					XMMatrixRotationZ(longitudeAngle * iLong)
				);
				v = XMVectorAdd(v, offset);
				XMStoreFloat3(&calculatedPos, v);

				vertices.push_back(calculatedPos);
				normals.push_back(XMFLOAT3{0.0f, 0.0f, 1.0f});
			}

			// fusilage vertices
			const auto iFusilage = static_cast<unsigned short>(vertices.size());
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				// near base
				{
					XMFLOAT3 calculatedPos;
					auto v = XMVector3Transform(
						base,
						XMMatrixRotationZ(longitudeAngle * iLong)
					);
					XMStoreFloat3(&calculatedPos, v);

					vertices.push_back(calculatedPos);
					normals.push_back(XMFLOAT3{calculatedPos.x, calculatedPos.y, 0.0f});
				}
				// far base
				{
					XMFLOAT3 calculatedPos;
					auto v = XMVector3Transform(
						base,
						XMMatrixRotationZ(longitudeAngle * iLong)
					);
					v = XMVectorAdd(v, offset);
					XMStoreFloat3(&calculatedPos, v);

					vertices.push_back(calculatedPos);
					normals.push_back(XMFLOAT3{calculatedPos.x, calculatedPos.y, 0.0f});
				}
			}

			std::vector<unsigned short> indices;

			// near base indices
			for (unsigned short iLong = 0; iLong < longDiv; iLong++)
			{
				const auto i = iLong;
				const auto mod = longDiv;
				// near
				indices.push_back(i + iBaseNear);
				indices.push_back(iCenterNear);
				indices.push_back((i + 1) % mod + iBaseNear);
			}

			// far base indices
			for (unsigned short iLong = 0; iLong < longDiv; iLong++)
			{
				const auto i = iLong;
				const auto mod = longDiv;
				// far
				indices.push_back(iCenterFar);
				indices.push_back(i + iBaseFar);
				indices.push_back((i + 1) % mod + iBaseFar);
			}

			// fuselage indices
			for (unsigned short iLong = 0; iLong < longDiv; iLong++)
			{
				const auto i = iLong * 2;
				const auto mod = longDiv * 2;
				indices.push_back(i + iFusilage);
				indices.push_back((i + 2) % mod + iFusilage);
				indices.push_back(i + 1 + iFusilage);
				indices.push_back((i + 2) % mod + iFusilage);
				indices.push_back((i + 3) % mod + iFusilage);
				indices.push_back(i + 1 + iFusilage);
			}

			for (unsigned int i = 0; i < vertices.size(); i++)
			{
				vbd.EmplaceBack(vertices[i], normals[i]);
			}

			return {std::move(vbd), std::move(indices)};
		}
	};
}
