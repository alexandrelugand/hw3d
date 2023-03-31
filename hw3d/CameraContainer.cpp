#include "stdafx.h"
#include "CameraContainer.h"

namespace Entities
{
	void CameraContainer::SpawnWindow(Graphics& gfx)
	{
		if (ImGui::Begin("Cameras"))
		{
			if (ImGui::BeginCombo("Active Camera", (*this)->GetName().c_str()))
			{
				for (int i = 0; i < cameras.size(); i++)
				{
					const bool isSelected = i == active;
					if (ImGui::Selectable(cameras[i]->GetName().c_str(), isSelected))
					{
						active = i;
					}
				}
				ImGui::EndCombo();
			}

			if (ImGui::BeginCombo("Controlled Camera", GetControlledCamera().GetName().c_str()))
			{
				for (int i = 0; i < cameras.size(); i++)
				{
					const bool isControlled = i == controlled;
					if (ImGui::Selectable(cameras[i]->GetName().c_str(), isControlled))
					{
						controlled = i;
					}
				}
				ImGui::EndCombo();
			}

			GetControlledCamera().SpawnControlWidgets(gfx);
		}
		ImGui::End();
	}

	void CameraContainer::Bind(Graphics& gfx) const noexcpt
	{
		GetActiveCamera().BindToGraphics(gfx);
	}

	void CameraContainer::AddCamera(std::unique_ptr<Camera> pCam)
	{
		cameras.push_back(std::move(pCam));
	}


	Camera* CameraContainer::operator->() const
	{
		return &GetActiveCamera();
	}

	void CameraContainer::LinkTechniques(Rgph::RenderGraph& rg) const
	{
		for (const auto& cam : cameras)
		{
			cam->LinkTechniques(rg);
		}
	}

	void CameraContainer::Submit() const
	{
		for (int i = 0; i < cameras.size(); i++)
		{
			if (i != active)
				cameras[i]->Submit();
		}
	}

	Camera& CameraContainer::GetActiveCamera() const
	{
		return *cameras[active];
	}

	Camera& CameraContainer::GetControlledCamera() const
	{
		return *cameras[controlled];
	}
}
