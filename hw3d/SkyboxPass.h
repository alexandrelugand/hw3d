#pragma once

namespace Rgph
{
	class SkyboxPass : public BindingPass
	{
	public:
		SkyboxPass(Graphics& gfx, std::string name);

		void BindMainCamera(Entities::Camera& camera) noexcept;
		void Execute(Graphics& gfx) const noexcpt override;
		void RenderWindow();

	private:
		bool useSphere = true;
		const Entities::Camera* pMainCamera = nullptr;
		std::shared_ptr<Bind::VertexBuffer> pCubeVertices;
		std::shared_ptr<Bind::IndexBuffer> pCubeIndices;
		unsigned int cubeCount;
		std::shared_ptr<Bind::VertexBuffer> pSphereVertices;
		std::shared_ptr<Bind::IndexBuffer> pSphereIndices;
		unsigned int sphereCount;
	};
}
