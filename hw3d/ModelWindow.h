#pragma once
#include "Forwards.h"

namespace Windowing
{
	class ModelWindow
	{
	public:
		void Show(Graphics& gfx, const char* windowName, Entities::Model& model, Entities::Node& root) noexcpt;
		XMMATRIX GetTransform() const noexcpt;

		Entities::Node* GetSelectedNode() const noexcept;

	private:
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

		Renderer::PSMaterialConstantFull skinMaterial;
		Renderer::PSMaterialConstantNoTex ringMaterial;
		std::unordered_map<int, TransformParameters> transforms;
	};
}
