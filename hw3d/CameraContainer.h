#pragma once

namespace Entities
{
	class CameraContainer
	{
	public:
		CameraContainer() = default;
		~CameraContainer() = default;

		void SpawnWindow(Graphics& gfx);
		void Bind(Graphics& gfx) const noexcpt;
		void AddCamera(std::shared_ptr<Camera> pCam);
		Camera& GetActiveCamera() const;
		Camera* operator->() const;

		void LinkTechniques(Rgph::RenderGraph& rg) const;
		void Submit(size_t channelFilter) const;

	private:
		Camera& GetControlledCamera() const;

		std::vector<std::shared_ptr<Camera>> cameras;
		int active = 0;
		int controlled = 0;
	};
}
