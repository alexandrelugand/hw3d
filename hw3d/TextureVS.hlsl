cbuffer CBuf
{
    matrix modelView;
    matrix modelViewProj;
};

struct VSOut
{
	float2 tex : TEXCOORD;
	float4 pos : SV_POSITION;
};

VSOut main(float3 pos : POSITION, float2 tex : TEXCOORD)
{
	VSOut vso;
    vso.pos = mul(float4(pos, 1.0f), modelViewProj);
	vso.tex = tex;
	return vso;
}