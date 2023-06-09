TextureCube tex : register(t4);
SamplerState sam : register(s0);

float4 main(float3 worldPos : POSITION) : SV_TARGET
{
    return tex.Sample(sam, worldPos);
}