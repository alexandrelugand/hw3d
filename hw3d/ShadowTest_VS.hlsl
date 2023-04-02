#include "Transform.hlsli"

cbuffer ShadowTransform
{
    matrix shadowView;
};

struct VSOut
{
    float3 viewFragPos : POSITION;
    float3 viewNormal : NORMAL;
    float3 viewTan : TANGENT;
    float3 viewBitan : BITANGENT;
    float2 tc : TEXCOORD;
    float4 shadowCamPos : SHADOWPOSITION;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float3 n : NORMAL, float2 tc : TEXCOORD, float3 tan : TANGENT, float3 bitan : BITANGENT)
{
    VSOut vso;
    vso.viewFragPos = (float3) mul(float4(pos, 1.0f), modelView);
    vso.viewNormal = mul(n, (float3x3) modelView);
    vso.viewTan = mul(tan, (float3x3) modelView);
    vso.viewBitan = mul(bitan, (float3x3) modelView);
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
    vso.tc = tc;
    const float4 shadowCamera = mul(float4(pos, 1.0f), model);
    const float4 shadowHomo = mul(shadowCamera, shadowView);
    vso.shadowCamPos = shadowHomo * float4(0.5f, -0.5f, 1.0f, 1.0f) + (float4(0.5f, 0.5f, 0.0f, 0.0f) * shadowHomo.w);
    return vso;
}