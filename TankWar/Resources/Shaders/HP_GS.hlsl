#include "Common.hlsli"

static const int vnum = 4;

static const float4 offset_array[vnum] =
{
    float4(-0.8f,  0.1f, 0.0f, 0.0f),   // ç∂è„
	float4( 0.8f,  0.1f, 0.0f, 0.0f),   // âEè„
	float4(-0.8f, -0.1f, 0.0f, 0.0f),   // ç∂â∫
	float4( 0.8f, -0.1f, 0.0f, 0.0f),   // âEâ∫
};

[maxvertexcount(vnum)]
void main(
	point PS_INPUT input[1],
	inout TriangleStream<PS_INPUT> output
)
{
    for (int i = 0; i < vnum; i++)
    {
        PS_INPUT element;
		
        element.Pos = input[0].Pos + mul(offset_array[i], matWorld);

        element.Pos = mul(element.Pos, matView);
        element.Pos = mul(element.Pos, matProj);

        element.Color = float4(input[0].Tex.x, 1.0f, 1.0f, 1.0f);
        element.Tex.x =  offset_array[i].x + 0.5f;
        element.Tex.y = -offset_array[i].y + 0.5f;

        output.Append(element);
    }
    output.RestartStrip();
}