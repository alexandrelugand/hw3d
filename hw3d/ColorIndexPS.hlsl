cbuffer CBuf
{
	float4 colors[8];
}

float4 main(uint tid : SV_PRIMITIVEID) : SV_TARGET
{
	return colors[(tid / 2) % 8];
}