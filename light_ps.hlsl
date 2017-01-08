Texture2D tex;
SamplerState state;

#define MAXLIGHTS 10

cbuffer DATA
{
	float4 spotlight_colour[MAXLIGHTS];
	float4 ambient_min;
	float4 ambient_max;
	float3 ambient_direction;
	float padding;
	int spotlight_count;
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
	// ambient
	float4 ambient_colour = ambient_min;
	float ambient_intensity = dot(input.normal, -ambient_direction);

	if (ambient_intensity > 0.0f)
	{
		float4 difference = ambient_max - ambient_min;
		ambient_colour += (difference * ambient_intensity);
	}

	float4 total = ambient_colour;

	// spotlights
	for (int i = 0U; i < spotlight_count; ++i)
	{
		float spotlight_intensity = dot(input.normal, input.light_positions[i]);
		total += saturate(spotlight_colour[i] * spotlight_intensity);
	}

	// multiply by the texture pixel
	total = saturate(total * tex.Sample(state, input.tex));
	total.w = 1.f;

	return total;
}