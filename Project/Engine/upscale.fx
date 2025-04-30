#ifndef _UPSCALE
#define _UPSCALE

#include "value.fx"
#include "struct.fx"

Texture2D<float4> g_InputTex : register(t20); // 블러 텍스처
RWTexture2D<float4> g_OutputTex : register(u0); // 원본 해상도 텍스처

#define OUTPUT_WIDTH  g_int_0
#define OUTPUT_HEIGHT g_int_1
#define INPUT_WIDTH   g_int_2
#define INPUT_HEIGHT  g_int_3

[numthreads(32, 32, 1)]
void CS_UpScale(uint3 _ID : SV_DispatchThreadID)
{
    if (_ID.x >= OUTPUT_WIDTH || _ID.y >= OUTPUT_HEIGHT)
        return;

    float2 vUV = float2((_ID.x + 0.5f) / (float) OUTPUT_WIDTH, (_ID.y + 0.5f) / (float) OUTPUT_HEIGHT);
 
    float4 vColor = g_InputTex.SampleLevel(g_sam_0, vUV, 0);
    
    g_OutputTex[_ID.xy] = vColor;
}

#endif