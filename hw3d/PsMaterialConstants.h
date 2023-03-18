#pragma once

namespace Renderer
{
	struct PSMaterialConstantDiffuse
	{
		float specularIntensity;
		float specularPower;
		float padding[2];
	};

	struct PSMaterialConstantDiffNorm
	{
		float specularIntensity;
		float specularPower;
		BOOL normalMapEnabled = TRUE;
		float padding[1];
	};

	struct PSMaterialConstantFull
	{
		BOOL normalMapEnabled = TRUE;
		BOOL specularMapEnabled = TRUE;
		BOOL hasGlossMap = FALSE;
		float specularPower = 3.1f;
		XMFLOAT3 specularColor = {0.75f, 0.75f, 0.75f};
		float specularMapWeight = 0.671f;
	};

	struct PSMaterialConstantNoTex
	{
		XMFLOAT4 materialColor = {0.447970f, 0.327254f, 0.176283f, 1.0f};
		XMFLOAT4 specularColor = {0.65f, 0.65f, 0.65f, 1.0f};
		float specularPower = 120.0f;
		float padding[3];
	};
}
