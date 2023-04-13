float main(float4 pos : SV_POSITION, float4 viewPos : POSITION) : SV_TARGET
{
    //fp is locked to 100
    float bias = 0.0005 + (pos.z - viewPos.w / pos.w);
    return saturate(length(viewPos.xyz) / 100.0f + bias);
}