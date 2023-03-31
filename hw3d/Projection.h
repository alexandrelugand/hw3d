#pragma once
#include "Frustum.h"

class Projection
{
public:
	Projection(Graphics& gfx, float width, float height, float nearZ, float farZ);
	XMMATRIX GetMatrix() const noexcept;
	void RenderWidgets(Graphics& gfx);
	void SetPos(XMFLOAT3 pos) noexcept;
	void SetRot(XMFLOAT3 rot) noexcept;
	void LinkTechniques(Rgph::RenderGraph& rg);
	void Submit() const;
	void Reset(Graphics& gfx);

private:
	float homeWidth;
	float homeHeight;
	float homeNearZ;
	float homeFarZ;

	float width;
	float height;
	float nearZ;
	float farZ;
	Draw::Frustum frustum;
};
