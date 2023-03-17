#pragma once

using ElementType = Dvtx::VertexLayout::ElementType;

namespace Geometry
{
	class Cone
	{
	public:
		static IndexedTriangleList MakeTesselated(int longDiv)
		{
			assert(longDiv >= 3);

			const auto layout = Dvtx::VertexLayout{}
				.Append(ElementType::Position3D);
			Dvtx::VertexBufferDescriptor vbd{std::move(layout)};

			std::vector<XMFLOAT3> vertices;

			const auto base = XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
			const float longitudeAngle = 2.0f * PI / longDiv;

			// base vertices
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				XMFLOAT3 calculatedPos;
				auto v = XMVector3Transform(
					base,
					XMMatrixRotationZ(longitudeAngle * iLong)
				);
				XMStoreFloat3(&calculatedPos, v);
				vertices.push_back(calculatedPos);
			}

			// the center
			XMFLOAT3 center{0.0f, 0.0f, -1.0f};
			vertices.push_back(center);
			const auto iCenter = static_cast<unsigned short>(vertices.size() - 1);

			// the tip :darkness:
			XMFLOAT3 tip{0.0f, 0.0f, 1.0f};
			vertices.push_back(tip);
			const auto iTip = static_cast<unsigned short>(vertices.size() - 1);

			// base indices
			std::vector<unsigned short> indices;
			for (unsigned short iLong = 0; iLong < longDiv; iLong++)
			{
				indices.push_back(iCenter);
				indices.push_back((iLong + 1) % longDiv);
				indices.push_back(iLong);
			}

			// cone indices
			for (unsigned short iLong = 0; iLong < longDiv; iLong++)
			{
				indices.push_back(iLong);
				indices.push_back((iLong + 1) % longDiv);
				indices.push_back(iTip);
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

			Dvtx::VertexBufferDescriptor vbd{std::move(layout)};

			std::vector<XMFLOAT3> vertices;
			std::vector<XMFLOAT3> normals;

			const auto base = XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
			const float longitudeAngle = 2.0f * PI / longDiv;

			// cone vertices
			const auto iCone = static_cast<unsigned short>(vertices.size());
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				const float thetas[] = {
					longitudeAngle * iLong,
					longitudeAngle * (((iLong + 1) == longDiv) ? 0 : (iLong + 1))
				};
				XMFLOAT3 top{0.0f, 0.0f, 1.0f};
				vertices.push_back(top);

				for (auto theta : thetas)
				{
					XMFLOAT3 calculatedPos;
					const auto v = XMVector3Transform(
						base,
						XMMatrixRotationZ(theta)
					);
					XMStoreFloat3(&calculatedPos, v);
					vertices.push_back(calculatedPos);
				}
			}

			// base vertices
			const auto iBaseCenter = static_cast<unsigned short>(vertices.size());
			XMFLOAT3 baseCenter{0.0f, 0.0f, -1.0f};
			vertices.push_back(baseCenter);

			const auto iBaseEdge = static_cast<unsigned short>(vertices.size());
			for (int iLong = 0; iLong < longDiv; iLong++)
			{
				XMFLOAT3 calculatedPos;
				auto v = XMVector3Transform(
					base,
					XMMatrixRotationZ(longitudeAngle * iLong)
				);
				XMStoreFloat3(&calculatedPos, v);
				vertices.push_back(calculatedPos);
			}

			std::vector<unsigned short> indices;

			// cone indices
			for (unsigned short i = 0; i < longDiv * 3; i++)
			{
				indices.push_back(i + iCone);
			}

			// base indices
			for (unsigned short iLong = 0; iLong < longDiv; iLong++)
			{
				indices.push_back(iBaseCenter);
				indices.push_back((iLong + 1) % longDiv + iBaseEdge);
				indices.push_back(iLong + iBaseEdge);
			}

			IndexedTriangleList::SetNormalsIndependentFlat(vertices, indices, normals);

			for (unsigned int i = 0; i < vertices.size(); i++)
			{
				vbd.EmplaceBack(vertices[i], normals[i]);
			}

			return {std::move(vbd), std::move(indices)};
		}
	};
}
