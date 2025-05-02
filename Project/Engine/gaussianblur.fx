#ifndef _GAUSSIANBLUR
#define _GAUSSIANBLUR

#include "value.fx"
#include "struct.fx"

#define TEX_WIDTH  g_int_0
#define TEX_HEIGHT g_int_1

Texture2D<float4>   g_InputTex  : register(t20);
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

#define KERNEL_HALF 6

[numthreads(128, 1, 1)]
void CS_VerticalFilter(uint3 _ID : SV_DispatchThreadID)
{
    int x = _ID.x;
    int y = _ID.y;

    if (x >= TEX_WIDTH || y >= TEX_HEIGHT)
        return;

    float4 vResult = 0;

    [unroll]
    for (int i = -KERNEL_HALF; i <= KERNEL_HALF; ++i)
    {
        int OffsetY = clamp(y + i, 0, TEX_HEIGHT - 1);
        vResult += g_InputTex.Load(int3(x, OffsetY, 0)) * SampleWeights[i + KERNEL_HALF];
    }

    g_OutputTex[int2(x, y)] = float4(vResult.rgb, 1.0f);
}
#endif