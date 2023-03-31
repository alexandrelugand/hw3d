#include "ShaderOps.hlsli"
#include "LightVectorData.hlsli"
#include "PointLight.hlsli"

cbuffer ObjectCBuf
{
    bool useGlossAlpha;
    bool useSpecularMap;
    float3 specularColor;
    float specularWeight;
    float specularGloss;
};

Texture2D tex;
Texture2D spec;
SamplerState splr;

float4 main(float3 viewFragPos : POSITION, float3 viewNormal : NORMAL, float2 tc : TEXCOORD) : SV_TARGET
{
    //// sample diffuse texture
    //float4 dtex = tex.Sample(splr, tc);
    
    //#ifdef MASK_BOI
    //// bail if highly translucent
    //clip(dtex.a < 0.1f ? -1 : 1); //skip current pixel

    //// flip normal when backface
    //if (dot(viewNormal, viewFragPos) >= 0.0f)
    //{
    //    viewNormal = -viewNormal;
    //}
    //#endif

    // normalize the mesh normal
    viewNormal = normalize(viewNormal);

	// fragment to light vector data
    const LightVectorData lv = CalculateLightVectorData(viewLightPos, viewFragPos);

 

    // specular parameters
    float specularPowerLoaded = specularGloss;
    const float4 specularSample = spec.Sample(splr, tc);
    float3 specularReflectionColor;
    if (useSpecularMap)
    {
        specularReflectionColor = specularSample.rgb;
    }
    else
    {
        specularReflectionColor = specularColor;
    }

    if (useGlossAlpha)
    {
        specularPowerLoaded = pow(2.0f, specularSample.a * 13.0f);
    }

	// attenuation
    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);

	// diffuse light
    const float3 diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, viewNormal);

    // specular reflected
    const float3 specularReflected = Speculate(
        diffuseColor * specularReflectionColor, specularWeight, viewNormal,
        lv.vToL, viewFragPos, att, specularPowerLoaded
    );

	// final color = attenuate diffuse & ambient by diffuse texture color and add specular reflected
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specularReflected), 1.0f);
}