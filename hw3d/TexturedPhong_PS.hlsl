#include "ShaderOps.hlsli"
#include "LightVectorData.hlsli"
#include "PointLight.hlsli"

cbuffer ObjectCBuf
{
    float specularIntensity;
    float specularPower;
    float padding[2];
}

Texture2D tex;
SamplerState splr;

float4 main(float3 viewFragPos : POSITION, float3 viewNormal : NORMAL, float2 tc : TEXCOORD) : SV_TARGET
{
    // normalize the mesh normal
    viewNormal = normalize(viewNormal);

	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, viewFragPos);

    // attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);

    // diffuse
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, viewNormal);

    // specular
    const float3 specular = Speculate(float3(0.65f, 0.65f, 0.65f), diffuseIntensity * specularIntensity, viewNormal,
        lv.vToL, viewFragPos, att, specularPower
    );

    // final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f);
}