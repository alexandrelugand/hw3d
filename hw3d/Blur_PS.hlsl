Texture2D tex;
SamplerState splr;

static const int r = 12;
static const float divisor = (2 * r + 1) * (2 * r + 1);

float4 main(float2 uv : TEXCOORD) : SV_TARGET
{
    uint width, height;
    tex.GetDimensions(width, height);

    //using to get position of pixel in texture coordinate
    const float dx = 1.0f / width;
    const float dy = 1.0f / height;
    float accAlpha = 0.0f;
    float3 maxColor = float3(0.0f, 0.0f, 0.0f);

    //Blur effect using a stencil of 3x3 pixels
    for (int y = -r; y <= r; y++)
    {
        for (int x = -r; x <= r; x++)
        {
            const float2 tc = uv + float2(dx * x, dy * y);
            const float4 s = tex.Sample(splr, tc).rgba;
            accAlpha += s.a;
            maxColor = max(s.rgb, maxColor); //keep only the max value, blur will be do with alpha channel and blender
        }
    }

    // divide by divisor to restore illumination
    return float4(maxColor, accAlpha / divisor);
}