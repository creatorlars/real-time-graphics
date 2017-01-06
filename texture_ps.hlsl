Texture2D tex;
SamplerState state;

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
	return tex.Sample(state, input.tex);
}