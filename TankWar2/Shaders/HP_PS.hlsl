#include "HP.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//‰æ‘œ•\¦
    float4 output = tex.Sample(samLinear, input.Tex);

	//^‚Á”’‚È”Âƒ|ƒŠƒSƒ“
    float4 outputw = float4(1, 1, 1, 1);
	
    output.a *= lerp(1.0f, 0.0f, step(input.Color.x, input.Tex.x));
	
    return output;
}