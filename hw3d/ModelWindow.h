#pragma once
#include "DynamicConstant.h"

namespace Windowing
{
	class ModelWindow
	{
	public:
		void Show(Graphics& gfx, const char* windowName, Entities::Model& model, Entities::Node& root) noexcpt;

		void ApplyParameters() noexcpt;
		XMMATRIX GetTransform() const noexcpt;

	private:
		const Dcb::Buffer& GetMaterial() const noexcpt;
		bool TransformDirty() const noexcpt;
		void ResetTransformDirty() noexcpt;
		bool MaterialDirty() const noexcpt;
		void ResetMaterialDirty() noexcpt;
		bool IsDirty() const noexcpt;

		Entities::Node* pSelectedNode;

		struct TransformParameters
		{
			float roll = 0.0f;
			float pitch = 0.0f;
			float yaw = 0.0f;
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
		};

		struct NodeData
		{
			TransformParameters tranformParams;
			bool transformParamsDirty;
			std::optional<Dcb::Buffer> materialCbuf;
			bool materialCbufDirty;
		};

		std::unordered_map<int, NodeData> transforms;
	};
}
