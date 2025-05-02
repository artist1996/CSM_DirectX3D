#ifndef _SHADOWBLUR
#define _SHADOWBLUR

#include "value.fx"

Texture2D<float4> g_InputTex   : register(t20);
RWTexture2D<float4> g_OutputTex : register(u0);

#define TEX_WIDTH  g_int_0
#define TEX_HEIGHT g_int_1

[numthreads(32,32,1)]
void CS_SoftShadowBlur(uint3 _ID : SV_DispatchThreadID)
{
    if(TEX_WIDTH <= _ID.x || TEX_HEIGHT <= _ID.y)
        return;
    
    float4 vCenter  = g_InputTex[_ID.xy];
    float4 vLeft    = g_InputTex[_ID.xy + int2(-1, 0)];
    float4 vRight   = g_InputTex[_ID.xy + int2(1, 0)];
    float4 vUp      = g_InputTex[_ID.xy + int2(0, -1)];
    float4 vDown    = g_InputTex[_ID.xy + int2(0, 1)];
    
    
    bool IsEdge = any(  abs(vCenter - vLeft) > 0.001f  ||
                        abs(vCenter - vRight) > 0.001f ||
                        abs(vCenter - vUp) > 0.001f    ||
                        abs(vCenter - vDown) > 0.001f);
    
    if(IsEdge)
    {
        float4 vSum = float4(0.f, 0.f, 0.f, 0.f);
        for (int y = -1; y <= 1; ++y)
        {
            for (int x = -1; x <= 1; ++x)
            {
                vSum += g_InputTex[_ID.xy + int2(x, y)];
            }
        }
        g_OutputTex[_ID.xy] = vSum / 9.f;
    }
    
    else
    {
        g_OutputTex[_ID.xy] = vCenter;
    }
}

#endif