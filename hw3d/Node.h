#pragma once

namespace Entities
{
	class Node
	{
		friend class Model;

	public:
		Node(int id, const std::string& name, std::vector<Mesh*> meshPtrs, const XMMATRIX& transform_in) noexcpt;

		void Draw(Graphics& gfx, FXMMATRIX accumulatedTransform) const noexcpt;
		void SetAppliedTransform(FXMMATRIX transform) noexcpt;
		int GetId() const noexcept { return id; }
		void ShowTree(Node*& pSelectedNode) const noexcpt;
		void ResetNode() noexcpt;

		template <class T>
		bool ControlMaterial(Graphics& gfx, T& c)
		{
			if (meshPtrs.empty())
			{
				return false;
			}

			if constexpr (std::is_same_v<T, Renderer::PSMaterialConstantFull>)
			{
				if (auto pcb = meshPtrs.front()->QueryBindable<Bind::PixelConstantBuffer<T>>())
				{
					ImGui::Text("Material");

					bool normalMapEnabled = static_cast<bool>(c.normalMapEnabled);
					ImGui::Checkbox("Norm Map", &normalMapEnabled);
					c.normalMapEnabled = normalMapEnabled ? TRUE : FALSE;

					bool specularMapEnabled = static_cast<bool>(c.specularMapEnabled);
					ImGui::Checkbox("Spec Map", &specularMapEnabled);
					c.specularMapEnabled = specularMapEnabled ? TRUE : FALSE;

					bool hasGlossMap = static_cast<bool>(c.hasGlossMap);
					ImGui::Checkbox("Gloss Alpha", &hasGlossMap);
					c.hasGlossMap = hasGlossMap ? TRUE : FALSE;

					ImGui::SliderFloat("Spec Weight", &c.specularMapWeight, 0.0f, 2.0f);
					ImGui::SliderFloat("Spec Pow", &c.specularPower, 0.0f, 1000.0f, "%f");
					ImGui::ColorPicker3("Spec Color", reinterpret_cast<float*>(&c.specularColor));

					pcb->Update(gfx, c);
					return true;
				}
			}
			else if constexpr (std::is_same_v<T, Renderer::PSMaterialConstantNoTex>)
			{
				if (auto pcb = meshPtrs.front()->QueryBindable<Bind::PixelConstantBuffer<T>>())
				{
					ImGui::Text("Material");
					ImGui::ColorPicker3("Spec Color", reinterpret_cast<float*>(&c.specularColor));
					ImGui::SliderFloat("Spec Pow", &c.specularPower, 0.0f, 1000.0f, "%f");
					ImGui::ColorPicker3("Diff Color", reinterpret_cast<float*>(&c.materialColor));

					pcb->Update(gfx, c);
					return true;
				}
			}
			return false;
		}

	private:
		void AddChild(std::unique_ptr<Node> pChild) noexcpt;

		std::string name{};
		int id = 0;
		std::vector<Mesh*> meshPtrs{};
		std::vector<std::unique_ptr<Node>> childPtrs{};
		XMFLOAT4X4 transform{};
		XMFLOAT4X4 appliedTransform{};
	};
}
