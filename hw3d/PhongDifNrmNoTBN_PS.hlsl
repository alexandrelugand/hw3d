#include "ShaderOps.hlsli"
#include "LightVectorData.hlsli"
#include "PointLight.hlsli"
#include "Transform.hlsli"

cbuffer ObjectCBuf : register(b2)
{
    float3 specularColor;
    float specularWeight;
    float specularGloss;
    bool useNormalMap;
    float normalMapWeight;
};

Texture2D tex;
Texture2D nmap : register(t2);
SamplerState splr;

float4 main(float3 viewFragPos : POSITION, float3 viewNormal : NORMAL, float2 tc : TEXCOORD) : SV_TARGET
{
    // sample normal from map if normal mapping enabled
    if (useNormalMap)
    {
        // sample and unpack normal data
        const float3 normalSample = nmap.Sample(splr, tc).xyz;
        const float3 objectNormal = normalSample * 2.0f - 1.0f;
        // bring normal from object space into view space
        viewNormal = normalize(mul(objectNormal, (float3x3) modelView));
    }

	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, viewFragPos);

	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);

	// diffuse intensity
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, viewNormal);

	// specular
    const float3 specular = Speculate(
        diffuseColor * diffuseIntensity * specularColor, specularWeight, viewNormal,
        lv.vToL, viewFragPos, att, specularGloss
    );

	// final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f);
}