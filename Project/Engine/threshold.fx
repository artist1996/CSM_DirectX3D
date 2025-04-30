#ifndef _THRESHOLD
#define _THRESHOLD

#include "value.fx"
#include "struct.fx"

#define TEX_WIDTH    g_int_0
#define TEX_HEIGHT   g_int_1

Texture2D<float4>       g_InputTex : register(t20);
RWTexture2D<float4>     g_OutputTex : register(u0);


[numthreads(32, 32, 1)]
void CS_Threshold(uint3 _ID : SV_DispatchThreadID)
{
    if (_ID.x >= TEX_WIDTH || _ID.y >= TEX_HEIGHT)
        return;

    float4 color = g_InputTex[_ID.xy];

    color.rgb = pow(color.rgb, 2.2f); // 감마 제거
    float luminance = dot(color.rgb, float3(0.2126f, 0.7152f, 0.0722f));
    g_OutputTex[_ID.xy] = (luminance > 1.f) ? color : float4(0.f, 0.f, 0.f, 0.f);
    
}

#endif