float4 VertexShaderMain(float4 pos : POSITION) : SV_POSITION
{
	return pos;
}

float4 PixelShaderMain(float4 Pos : SV_POSITION) : SV_Target
{
	return float4(1.0f, 1.0f, 0.0f, 1.0f);
}

float4 main(float4 pos : POSITION) : SV_POSITION
{
	return pos;
}