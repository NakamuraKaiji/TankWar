#include "HP.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	//画像表示
    float4 output = tex.Sample(samLinear, input.Tex);

	//真っ白な板ポリゴン
    float4 outputw = float4(1, 1, 1, 1);
	
    output.a *= lerp(1.0f, 0.0f, step(input.Color.x, input.Tex.x));
	
    return output;
}