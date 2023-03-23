#include "ShaderOps.hlsli"
#include "LightVectorData.hlsli"
#include "PointLight.hlsli"

cbuffer ObjectCBuf
{
    float specularPower;
    bool hasGloss;
    float specularMapWeight;
};

Texture2D tex;
Texture2D spec;
SamplerState splr;

float4 main(float3 viewFragPos : POSITION, float3 viewNormal : NORMAL, float2 tc : TEXCOORD) : SV_TARGET
{
    // normalize the mesh normal
    viewNormal = normalize(viewNormal);

	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, viewFragPos);

    // specular parameters
    float specularPowerLoaded = specularPower;
    const float4 specularSample = spec.Sample(splr, tc);
    const float3 specularReflectionColor = specularSample.rgb * specularMapWeight;

    if (hasGloss)
    {
        specularPowerLoaded = pow(2.0f, specularSample.a * 13.0f);
    }

	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);

	// diffuse light
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, viewNormal);

    // specular reflected
    const float3 specularReflected = Speculate(
        specularReflectionColor, diffuseIntensity, viewNormal,
        lv.vToL, viewFragPos, att, specularPowerLoaded
    );

	// final color = attenuate diffuse & ambient by diffuse texture color and add specular reflected
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specularReflected), 1.0f);
}