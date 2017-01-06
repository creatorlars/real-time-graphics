cbuffer MATRICES
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VS_INPUT
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// calculate the position of the vertex
	VS_OUTPUT output;
	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	return output;
}