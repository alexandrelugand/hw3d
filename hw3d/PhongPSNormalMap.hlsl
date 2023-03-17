cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer ObjectCBuf
{
    float specularIntensity;
    float specularPower;
    bool normalMapEnabled;
    float padding[1];
};

cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProj;
};

Texture2D tex;
Texture2D nmap;
SamplerState splr;

float4 main(float3 camPos : POSITION, float3 n : NORMAL, float2 tc: TEXCOORD) : SV_TARGET
{
    // sample normal from map if normal mapping enabled
    if (normalMapEnabled)
    {
        const float3 normalSample = nmap.Sample(splr, tc).xyz;
        n.x = normalSample.x * 2.0f - 1.0f;  //Coordinate origin offset (value between -1 / +1, not 0 / +1)
        n.y = -normalSample.y * 2.0f + 1.0f; //Coordinate origin offset (value between -1 / +1, not 0 / +1). Negate to adjust coordinate in DirectX format (Y axis opposite then OpenGL)
        n.z = -normalSample.z; //Negate to redirect normal towards camera
        n = mul(n, (float3x3) modelView);
    }

	// fragment to light vector data
    const float3 vToL = lightPos - camPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;

	// diffuse attenuation
    const float att = 1.0f / (attConst + attLin * distToL + attQuad * (distToL * distToL));

	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.0f, dot(dirToL, n));

    // reflected light vector
    const float3 w = n * dot(vToL, n);
    const float3 r = w * 2.0f - vToL;

    // calculate specular intensity based on angle between viewing vector, narrow with power function
    const float3 specular = att * (diffuseColor * diffuseIntensity) * specularIntensity * pow(max(0.0f, dot(normalize(-r), normalize(camPos))), specularPower);

	// final color
    return float4(saturate((diffuse + ambient) * tex.Sample(splr, tc).rgb + specular), 1.0f);
}