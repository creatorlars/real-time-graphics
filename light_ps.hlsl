Texture2D tex;
SamplerState state;

cbuffer DATA
{
	float4 ambient;
	float4 diffuse;
	float3 direction;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	// calculate light intensity
	float intensity = dot(input.normal, -direction);
	
	// calculate diffuse colour
	float4 color = ambient;
	if (intensity > 0.0f)
	{
		color += (diffuse * intensity);
	}
	
	// multiply by the texture pixel
	color = color * tex.Sample(state, input.tex);
	
	return color;
}