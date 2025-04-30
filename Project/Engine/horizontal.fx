#ifndef _HORIZONTALBLUR
#define _HORIZONTALBLUR

#include "value.fx"
#include "struct.fx"

#define TEX_WIDTH  g_int_0
#define TEX_HEIGHT g_int_1

Texture2D<float4> g_InputTex : register(t20);
RWTexture2D<float4> g_OutputTex : register(u0);

static const float SampleWeights[13] =
{
    0.002216,
    0.008764,
    0.026995,
    0.064759,
    0.120985,
    0.176033,
    0.199471,
    0.176033,
    0.120985,
    0.064759,
    0.026995,
    0.008764,
    0.002216,
};

#define kernelhalf 6

[numthreads(1, 128, 1)]
void CS_HorizFilter(uint3 _ID : SV_DispatchThreadID)
{
    int x = _ID.x;
    int y = _ID.y;

    if (x >= TEX_WIDTH || y >= TEX_HEIGHT)
        return;

    float4 result = 0;

    [unroll]
    for (int i = -kernelhalf; i <= kernelhalf; ++i)
    {
        int offsetX = clamp(x + i, 0, TEX_WIDTH - 1);
        result += g_InputTex.Load(int3(offsetX, y, 0)) * SampleWeights[i + kernelhalf];
    }

    g_OutputTex[int2(x, y)] = float4(result.rgb, 1.0f);
}
#endif