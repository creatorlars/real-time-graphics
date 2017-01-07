Texture2D tex;
SamplerState state;

#define MAXLIGHTS 10

cbuffer DATA
{
	float4 ambient[MAXLIGHTS];
	float4 diffuse[MAXLIGHTS];
	float3 direction[MAXLIGHTS];
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
	for (int i = 0U; i < count; ++i)
	{
		/*
		// calculate light intensity
		float intensity = dot(input.normal, -direction[i]);

		// calculate diffuse colour
		float4 colour = ambient[i];
		if (intensity > 0.0f)
		{
			colour += (diffuse[i] * intensity);
		}
		total += colour;
		*/

		float intensity = dot(input.normal, input.light_positions[i]);
		total += diffuse[i] * intensity;
	}

	// multiply by the texture pixel
	total = total * tex.Sample(state, input.tex);
	total.w = 1.f;

	return total;
}