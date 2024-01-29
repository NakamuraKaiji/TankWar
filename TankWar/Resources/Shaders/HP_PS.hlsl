#include "Common.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//‰æ‘œ•\Ž¦
    float4 output = tex.Sample(samLinear, input.Tex);
	
    output.a *= lerp(1.0f, 0.0f, step(input.Color.x, input.Tex.x));
    	
    return output;
}