#define MAXLIGHTS 10

cbuffer MATRICES
{
	matrix world;
	matrix view;
	matrix projection;
};

cbuffer DATA
{
	float4 light_positions[MAXLIGHTS];
	int count;
};

struct VS_INPUT
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 light_positions[MAXLIGHTS] : TEXCOORD1;
};

VS_OUTPUT main(VS_INPUT input)
{
	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	VS_OUTPUT output;
	output.position = mul(input.position, world);

	for (int i = 0; i < count; ++i)
	{
		output.light_positions[i].xyz = normalize(light_positions[i].xyz - output.position.xyz);
	}

	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);

	// Store the texture coordinates for the pixel shader.
	output.tex = input.tex;

	// calculate the normal vector
	output.normal = normalize(mul(input.normal, (float3x3)world));

	return output;
}