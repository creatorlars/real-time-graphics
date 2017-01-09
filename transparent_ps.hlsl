Texture2D tex;
SamplerState state;

cbuffer DATA
{
	float blend;
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	// set the alpha to the difference between target and source alpha
	float4 color = tex.Sample(state, input.tex);	
	color.a = blend * color.a;

	return color;
}
