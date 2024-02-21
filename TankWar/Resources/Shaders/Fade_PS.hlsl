
Texture2D<float4> Texture : register(t0);
sampler TextureSampler : register(s0);

Texture2D<float4> MaskTexture : register(t1);

cbuffer Parameters : register(b1)
{
    float Rate;
};

float4 main(float4 color : COLOR0, float2 texCoord : TEXCOORD0) : SV_Target0
{
    float4 col = Texture.Sample(TextureSampler, texCoord) * color;
    float4 mask = MaskTexture.Sample(TextureSampler, texCoord);

    // マスク用テクスチャを使用
    // 中心から消える
    float rate = saturate((mask.x - 1.0f) + Rate * 2.0f);
    // 画面端から消える
    //float rate = saturate(-mask.x + Rate * 2.0f);
    
    return float4(col.rgb, rate);
}
