Texture2D tex;
SamplerState state;

#define MAXLIGHTS 10

cbuffer DATA
{
	float4 spotlight_min[MAXLIGHTS];
	float4 spotlight_max[MAXLIGHTS];
	float4 ambient_min;
	float4 ambient_max;
	float3 ambient_direction;
	float padding;
	int count;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 light_positions[MAXLIGHTS] : TEXCOORD1;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 total;

	float ambient_intensity = dot(input.normal, -ambient_direction);
	
	float4 ambient_colour = ambient_min;
	if (ambient_intensity > 0.0f)
	{
		ambient_colour += (ambient_max * ambient_intensity);
	}
	total = ambient_colour;

	for (int i = 0U; i < count; ++i)
	{
		float intensity = dot(input.normal, input.light_positions[i]);
		total += spotlight_min[i] * intensity;
	}

	// multiply by the texture pixel
	total = total * tex.Sample(state, input.tex);
	total.w = 1.f;

	return total;
}