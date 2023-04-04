#include "Transform.hlsli"
#include "VShadow.hlsli"

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
    float4 shadowHomoPos : SHADOWPOSITION;
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
    vso.shadowHomoPos = ToShadowHomoSpace(pos, model);
    return vso;
}