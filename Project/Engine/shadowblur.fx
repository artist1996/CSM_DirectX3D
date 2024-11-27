#ifndef _SHADOWBLUR
#define _SHADOWBLUR

#include "value.fx"

#define SHADOW_TARGET g_tex_0

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV  : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV       : TEXCOORD;
};

VS_OUT VS_ShadowBlur(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
};

static float CrossFilter[13] = { 0.0561f, 0.1353f, 0.278f, 0.4868f, 0.7261f, 0.9231f, 1.f, 0.9231f, 0.7261f, 0.4868f, 0.278f, 0.1353f, 0.0561f };
static float Total = 6.2108f;

float4 PS_ShadowBlur(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
        
    float2 vUVStep = 1.f / g_Resolution;
    vUVStep *= 3.4f;
    
    for (int i = 0; i < 13; ++i)
    {
        float2 vUV = _in.vUV + float2(vUVStep.x * (-6 + i), 0.f);
        vColor += SHADOW_TARGET.Sample(g_sam_3, vUV) * CrossFilter[i];
        vUV = _in.vUV + float2(0.f, vUVStep.y * (-6 + i));
        vColor += SHADOW_TARGET.Sample(g_sam_3, vUV) * CrossFilter[i];
    }
       
    //for (int j = 0; j < 13; ++j)
    //{
    //    float2 vUV = _in.vUV + float2(0.f, vUVStep.y * (-6 + j));
    //    vColor += g_tex_0.Sample(g_sam_3, vUV) * CrossFilter[j];
    //}
   
    vColor /= Total;
    
    return vColor;
}

#endif