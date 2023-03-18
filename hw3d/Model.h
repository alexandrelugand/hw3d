#pragma once
#include "Mesh.h"
#include "Node.h"

namespace Entities
{
	class Model
	{
		class ModelException : public Hw3DException
		{
		public:
			ModelException(int line, const char* file, std::string note) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			const std::string& GetNote() const noexcept;

		private:
			std::string note;
		};

	public:
		Model(Graphics& gfx, const std::string& fileName);
		~Model() noexcept;

		void Draw(Graphics& gfx) const noexcpt;
		void ShowWindow(Graphics& gfx, const char* windowName = nullptr);
		void SetRootTransform(FXMMATRIX tf) const noexcept;

	private:
		std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const std::string& base, const aiMesh& mesh, const aiMaterial* const* pMaterials);
		std::unique_ptr<Node> ParseNode(int& nextId, const aiNode& node);

		std::unique_ptr<Node> pRoot = nullptr;
		std::unique_ptr<Windowing::ModelWindow> pWindow = nullptr;
		std::vector<std::unique_ptr<Mesh>> meshPtrs{};
	};
}
