#ifndef _DOWNSCALE
#define _DOWNSCALE

#include "value.fx"
#include "struct.fx"

#define INPUT_TEX_WIDTH    g_int_0
#define INPUT_TEX_HEIGHT   g_int_1
#define OUTPUT_TEX_WIDTH   g_int_2
#define OUTPUT_TEX_HEIGHT  g_int_3

Texture2D<float4>   g_InputTex  : register(t20);     // Threshold Texture
RWTexture2D<float4> g_OutputTex : register(u0);      // DownScale Texture  

[numthreads(32, 32, 1)]
void CS_DownScale(uint3 _ID : SV_DispatchThreadID)
{
    if (_ID.x >= OUTPUT_TEX_WIDTH || _ID.y >= OUTPUT_TEX_HEIGHT)
        return;

    float2 baseUV = float2(_ID.xy * 4);
    float4 sum = float4(0.f, 0.f, 0.f, 0.f);

    for (int y = 0; y < 4; ++y)
    {
        for (int x = 0; x < 4; ++x)
        {
            float2 uv = (baseUV + float2(x, y)) / float2(INPUT_TEX_WIDTH, INPUT_TEX_HEIGHT);
            uv = clamp(uv, 0.0, 1.0);
            sum += g_InputTex.SampleLevel(g_sam_0, uv, 0);
        }
    }

    sum /= 16.f;
    sum.a = 1.f;

    g_OutputTex[_ID.xy] = sum;
}
#endif