#include "stdafx.h"
#include "Projection.h"

Projection::Projection(Graphics& gfx, float width, float height, float nearZ, float farZ)
	: homeWidth(width),
	  homeHeight(height),
	  homeNearZ(nearZ),
	  homeFarZ(farZ),
	  frustum(gfx, width, height, nearZ, farZ)
{
	Reset(gfx);
}

XMMATRIX Projection::GetMatrix() const noexcept
{
	return XMMatrixPerspectiveLH(width, height, nearZ, farZ);
}

void Projection::RenderWidgets(Graphics& gfx)
{
	bool dirty = false;
	const auto dcheck = [&dirty](bool d) { dirty = dirty || d; };

	ImGui::Text("Projection");
	dcheck(ImGui::SliderFloat("Width", &width, 0.01f, 4.0f, "%.2f"));
	dcheck(ImGui::SliderFloat("Height", &height, 0.01f, 4.0f, "%.2f"));
	dcheck(ImGui::SliderFloat("Near Z", &nearZ, 0.01f, farZ - 0.01f, "%.2f"));
	dcheck(ImGui::SliderFloat("Far Z", &farZ, 0.01f, 400.0f, "%.2f"));

	if (dirty)
		frustum.SetVertices(gfx, width, height, nearZ, farZ);
}

void Projection::SetPos(XMFLOAT3 pos) noexcept
{
	frustum.SetPos(pos);
}

void Projection::SetRot(XMFLOAT3 rot) noexcept
{
	frustum.SetRot(rot);
}

void Projection::LinkTechniques(Rgph::RenderGraph& rg)
{
	frustum.LinkTechniques(rg);
}

void Projection::Submit() const
{
	frustum.Submit();
}

void Projection::Reset(Graphics& gfx)
{
	width = homeWidth;
	height = homeHeight;
	nearZ = homeNearZ;
	farZ = homeFarZ;
	frustum.SetVertices(gfx, width, height, nearZ, farZ);
}
