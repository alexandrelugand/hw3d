#include "Transform.hlsli"

struct VSOut
{
    float3 viewFragPos : POSITION;
    float3 viewNormal : NORMAL;
    float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float3 n : NORMAL)
{
    VSOut vso;
    vso.viewFragPos = (float3) mul(float4(pos, 1.0f), modelView);
    vso.viewNormal = mul(n, (float3x3) modelView);
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
    return vso;
}