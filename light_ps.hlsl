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
};

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 total = 0.f;
	for (int i = 0; i < count; ++i)
	{
		// calculate light intensity
		float intensity = dot(input.normal, -direction[i]);

		// calculate diffuse colour
		float4 colour = ambient[i];
		if (intensity > 0.0f)
		{
			colour += (diffuse[i] * intensity);
		}
		total += colour;
	}

	// multiply by the texture pixel
	total = total * tex.Sample(state, input.tex);

	return total;
}