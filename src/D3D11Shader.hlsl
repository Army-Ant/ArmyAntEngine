cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

Texture2D colorMap_ : register(t0);
SamplerState colorSampler_ : register(s0);

struct VS_Input_Triangle
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct PS_Input_Triangle
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

struct VS_Input_Texture
{
    float4 pos : POSITION;
    float2 tex0 : TEXCOORD0;
};

struct PS_Input_Texture
{
	float4 pos : SV_POSITION; 
	float2 tex0 : TEXCOORD0;
};

float4 main(float4 pos : POSITION) : SV_POSITION
{
	pos.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
	pos = mul(pos, worldMatrix);
	pos = mul(pos, viewMatrix);
	pos = mul(pos, projectionMatrix);
	return pos;
}

PS_Input_Triangle VertexShaderMain_Triangle(VS_Input_Triangle inp)
{
    PS_Input_Triangle output;
    // Change the position vector to be 4 units for proper matrix calculations.
    inp.pos.w = 1.0f;
    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.pos = mul(inp.pos, worldMatrix);
    output.pos = mul(output.pos, viewMatrix);
    output.pos = mul(output.pos, projectionMatrix);    
    // Store the input color for the pixel shader to use.
    output.color = inp.color;
    return output;
}

float4 PixelShaderMain_Triangle(PS_Input_Triangle inp) : SV_Target
{
    return inp.color;
}

PS_Input_Texture VertexShaderMain_Texture(VS_Input_Texture vertex)
{
    PS_Input_Texture output;
    

    // Change the position vector to be 4 units for proper matrix calculations.
	vertex.pos.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
	output.pos = mul(vertex.pos, worldMatrix);
	output.pos = mul(output.pos, viewMatrix);
	output.pos = mul(output.pos, projectionMatrix);

    // Store the texture coordinates for the pixel shader.
	output.tex0 = vertex.tex0;
    
	return output;
}

float4 PixelShaderMain_Texture(PS_Input_Texture frag) : SV_Target
{
	return colorMap_.Sample(colorSampler_, frag.tex0);
}