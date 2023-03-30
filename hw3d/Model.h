#pragma once
#include "Mesh.h"
#include "Node.h"

namespace Entities
{
	class Model
	{
	public:
		Model(Graphics& gfx, const std::string& name, const std::string& filePath, const float& scale = 1.0f, const XMFLOAT3& position = {0.0f, 0.0f, 0.0f}, const XMFLOAT3& rotation = {0.0f, 0.0f, 0.0f});
		~Model() noexcept;

		int GetId() const noexcept;
		const std::string& GetName() const noexcept;

		void Submit() const noexcpt;
		void SetRootTransform(FXMMATRIX tf) const noexcept;
		void Accept(Probes::ModelProbe& probe) const;
		void LinkTechniques(Rgph::RenderGraph& rg) const;

	private:
		int id;
		std::string name;
		std::unique_ptr<Node> pRoot = nullptr;
		std::vector<std::unique_ptr<Mesh>> meshPtrs{};

		static int GetNextId()
		{
			static int next = 1;
			return next++;
		}

		std::unique_ptr<Node> ParseNode(int& nodeId, const aiNode& node, float scale);
	};
}
