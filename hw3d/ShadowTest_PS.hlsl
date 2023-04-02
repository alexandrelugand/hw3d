#include "ShaderOps.hlsli"
#include "LightVectorData.hlsli"
#include "PointLight.hlsli"

cbuffer ObjectCBuf
{
    float3 specularColor;
    float specularWeight;
    float specularGloss;
    bool useNormalMap;
    float normalMapWeight;
};

Texture2D tex;
Texture2D nmap : register(t2);
Texture2D smap : register(t3);

SamplerState splr;
SamplerState ssam;

float4 main(float3 viewFragPos : POSITION, float3 viewNormal : NORMAL, float3 viewTan : TANGENT, float3 viewBitan : BITANGENT, float2 tc : TEXCOORD, float4 spos : SHADOWPOSITION) : SV_TARGET
{
    float3 diffuse;
    float3 specular;
    
    // shadow map test
    spos.xyz = spos.xyz / spos.w;
    if (smap.Sample(ssam, spos.xy).r > spos.z)
    {
        // normalize the mesh normal
        viewNormal = normalize(viewNormal);

	    // replace normal with mapped if normal mapping enabled
        if (useNormalMap)
        {
            const float3 mappedNormal = MapNormal(normalize(viewTan), normalize(viewBitan), viewNormal, tc, nmap, splr);
            viewNormal = lerp(viewNormal, mappedNormal, normalMapWeight);
        }

		// fragment to light vector data
	    const LightVectorData lv = CalculateLightVectorData(viewLightPos, viewFragPos);

		// attenuation
	    const float att = Attenuate(attConst, attLin, attQuad, lv.distToL);

		// diffuse
	    diffuse = Diffuse(diffuseColor, diffuseIntensity, att, lv.dirToL, viewNormal);

	    // specular
	    specular = Speculate(
	        diffuseColor * diffuseIntensity * specularColor, specularWeight, viewNormal,
	        lv.vToL, viewFragPos, att, specularGloss
	    );
    }
    else
    {
        diffuse = specular = float3(0.0f, 0.0f, 0.0f);
    }

	// final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f);
}