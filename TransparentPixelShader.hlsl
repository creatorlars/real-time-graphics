Texture2D shaderTexture;
SamplerState SampleType;

cbuffer TransparentBuffer
{
	float blendAmount;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
	// Sample the texture pixel at this location.
	float4 color = shaderTexture.Sample(SampleType, input.tex);
	
	// Set the alpha value of this pixel to the blending amount to create the
	// alpha blending effect.	
	color.a = blendAmount * color.a;

	return color;
}
