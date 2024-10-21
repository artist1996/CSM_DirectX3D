#ifndef _POSTPROCESS
#define POSTPROCESS

#include "value.fx"

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

float Damped_sine_wave(float t)
{
#define LAMBDA 20.f
#define OMEGA 8.f
    
    return 0.1f * exp(-LAMBDA * abs(t) * (sin(OMEGA * PI * t)));
    
};

VS_OUT VS_GrayFilter(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos.xy * 2.f, 0.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
};

float4 PS_GrayFilter(VS_OUT _in) : SV_Target
{    
    
    // GrayFilter
    //float4 vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    //float Average = (vColor.x + vColor.y + vColor.z) / 3.f;
    //vColor = float4(Average, Average, Average, 1.f);
    
    // Cos Distortion
    //float2 vUV = _in.vUV;
    //vUV.y += cos((vUV.x + g_EngineTime * 0.1f) * PI * 12.f) * 0.01f;
    //float4 vColor = g_tex_0.Sample(g_sam_0, vUV);
    
    // Noise
    //float2 vUV = _in.vUV;
    //vUV.x += g_EngineTime * 0.1f;
    //float4 vNoise = g_tex_3.Sample(g_sam_0, vUV);
    //
    //vNoise = (vNoise * 2.f - 1.f) * 0.01f;
    //
    //vUV = _in.vUV + vNoise.xy;
    //float4 vColor = g_tex_0.Sample(g_sam_0, vUV);
    //
    //return vColor;
    
    // ÁöÁöÁ÷
    //float2 vScreenUV = _in.vPosition.xy / g_Resolution;
    //
    //float2 vNoiseUV = vScreenUV;
    //
    //vNoiseUV.x = dot(vScreenUV.y + (g_EngineTime * 0.5f) * 10.f, vScreenUV.x + (g_EngineTime * 0.5f) * 10.f);
    //vNoiseUV.y = dot(vScreenUV.x + (g_EngineTime * 0.5f) * 10.f, vScreenUV.y + (g_EngineTime * 0.5f) * 10.f);

    //vNoiseUV.x = vScreenUV.x + sin(vScreenUV.y * 10.f + g_EngineTime) * 0.1f;
    //vNoiseUV.y = vScreenUV.y + cos(vScreenUV.x * 10.f + g_EngineTime) * 0.1f;
    
    // ºÒ±ÔÄ¢ ÀÏ··ÀÓ
    //float2 vScreenUV = _in.vPosition.xy / g_Resolution;
    //
    //float2 vNoiseUV = vScreenUV;
    //
    
    //float4 vNoise = g_tex_2.Sample(g_sam_0, vNoiseUV);
    //
    //vNoise = (vNoise * 2.f - 1.f) * 0.01f;
    //
    //vScreenUV = vScreenUV + vNoise.xy;
    //
    //float4 vColor = g_tex_0.Sample(g_sam_0, vScreenUV);

    float2 vScreenUV = _in.vPosition.xy / g_Resolution;
   
    float2 fCenter = float2(0.5f, 0.5f);
    vScreenUV += -fCenter;
    float fLength = length(vScreenUV);
    
    float2 Ver = vScreenUV / fLength;
    
    float Corr = Damped_sine_wave(fLength - g_EngineTime * 0.1f);
    
    vScreenUV = Ver * (fLength + Corr * clamp(0.6f - length(vScreenUV), 0.f, 0.6f));
    vScreenUV -= fCenter;
    
    float4 vColor = g_tex_0.Sample(g_sam_0, vScreenUV);
    
    return vColor;
};

VS_OUT VS_Distortion(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;
    
    return output;
};

float4 PS_Distortion(VS_OUT _in) : SV_Target
{
    // 1. Render Target Resolution
    // 2. Pixel Shader Pixel ÁÂÇ¥
        
   //float2 vScreenUV = _in.vPosition.xy / g_Resolution;
   //float2 vNoiseUV = vScreenUV;
   //
   //vNoiseUV.x += g_EngineTime * 0.1f;
   //
   //float4 vNoise = g_tex_3.Sample(g_sam_0, vNoiseUV);
   //
   //vNoise = (vNoise * 2.f - 1.f) * 0.01f;
   //vScreenUV = vScreenUV + vNoise.xy;
   //// Gray Filter
   //float4 vColor = g_tex_0.Sample(g_sam_0, vScreenUV);
   //
   //return vColor;
    
    //float2 vScreenUV = _in.vPosition.xy / g_Resolution;
    //
    //float2 vNoiseUV = vScreenUV;
    //
    ////vNoiseUV.x = dot(vScreenUV.y + (g_EngineTime * 0.5f) * 2.f, vScreenUV.x + (g_EngineTime * 0.5f) * 2.f);
    ////vNoiseUV.y = dot(vScreenUV.x + (g_EngineTime * 0.5f) * 2.f, vScreenUV.y + (g_EngineTime * 0.5f) * 2.f);
    //
    ////vNoiseUV.x = vScreenUV.y + (g_EngineTime * 0.5f) * 4.f;
    ////vNoiseUV.y = vScreenUV.x + (g_EngineTime * 0.5f) * 4.f;
    //
    //float4 vNoise = g_tex_1.Sample(g_sam_0, vNoiseUV);
    //
    //vNoise = (vNoise * 2.f - 1.f) * 0.005f;
    //
    //vScreenUV = vScreenUV + vNoise.xy;
    //
    //float4 vColor = g_tex_0.Sample(g_sam_0, vScreenUV);
    //
    //return vColor;
    
    //float2 vScreenUV = _in.vPosition.xy / g_Resolution;
    //
    //float2 vNoiseUV = vScreenUV;
    
    // ÁöÁöÁ÷
    //vNoiseUV.x = dot(vScreenUV.y + (g_EngineTime * 0.5f) * 10.f, vScreenUV.x + (g_EngineTime * 0.5f) * 10.f);
    //vNoiseUV.y = dot(vScreenUV.x + (g_EngineTime * 0.5f) * 10.f, vScreenUV.y + (g_EngineTime * 0.5f) * 10.f);
    
    // ºÒ±ÔÄ¢ ÀÏ··ÀÓ  
    //vNoiseUV.x = vScreenUV.x + sin(vScreenUV.y * 10.f + g_EngineTime) * 0.1f;
    //vNoiseUV.y = vScreenUV.y + cos(vScreenUV.x * 10.f + g_EngineTime) * 0.1f;
      
    // ºÒ²É?
    //float fSin = sin(vScreenUV.y * 10.f + g_EngineTime);
    //if(fSin == 0.f)
    //{
    //    fSin = 0.3f;
    //}
    //
    //vNoiseUV.y = vScreenUV.x + fSin * 0.1f;
    //
    //float4 vNoise = g_tex_2.Sample(g_sam_0, vNoiseUV);
    //
    //vNoise = (vNoise * 2.f - 1.f) * 0.01f;
    //
    //vScreenUV = vScreenUV + vNoise.xy;
    //
    //float4 vColor = g_tex_0.Sample(g_sam_0, vScreenUV);
    //
    //return vColor;
    
    // Ç³µ¢
    float2 vScreenUV = _in.vPosition.xy / g_Resolution;
    vScreenUV += -float2(0.5f, 0.5f);
    float fLength = length(vScreenUV);
    
    float2 Ver = vScreenUV / fLength;
    
    float Corr = Damped_sine_wave(fLength - g_EngineTime * 0.07f);
    
    vScreenUV = Ver * (fLength + Corr * clamp(1.f - length(vScreenUV), 0.f, 1.f));
    vScreenUV -= -float2(0.5f, 0.5f);
    
    float4 vColor = g_tex_0.Sample(g_sam_0, vScreenUV);
    
    return vColor;
};

// ==========================
// Blur Shader
// Mesh     : RectMesh
// DSTYPE   : NO_TEST_NO_WRITE
// g_tex_0  : Target
// ===========================
static float GaussianFilter[5][5] =
{
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.0219f, 0.0983f, 0.1621f, 0.0983f, 0.0219f,
    0.0133f, 0.0596f, 0.0983f, 0.0596f, 0.0133f,
    0.003f, 0.0133f, 0.0219f, 0.0133f, 0.003f,
};

static float CrossFilter[13] = { 0.0561f, 0.1353f, 0.278f, 0.4868f, 0.7261f, 0.9231f, 1.f, 0.9231f, 0.7261f, 0.4868f, 0.278f, 0.1353f, 0.0561f };
static float Total = 6.2108f;

struct VS_OUT_BLUR
{
    float4 vPosition : SV_Position;
    float2 vUV : TEXCOORD;
    float InstID : FOG;
};

VS_OUT_BLUR VS_Blur(VS_IN _in, uint _InstID : SV_InstanceID)
{
    VS_OUT_BLUR output = (VS_OUT_BLUR) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    output.InstID = _InstID;
    
    return output;
}

float4 PS_Blur(VS_OUT_BLUR _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
        
    float2 vUVStep = 1.f / g_Resolution;
    vUVStep *= 3.4f;
    
    if (_in.InstID == 0)
    {
        for (int i = 0; i < 13; ++i)
        {
            float2 vUV = _in.vUV + float2(vUVStep.x * (-6 + i), 0.f);
            vColor += g_tex_0.Sample(g_sam_2, vUV) * CrossFilter[i];
        }
    }
    else if (_in.InstID == 1)
    {
        for (int j = 0; j < 13; ++j)
        {
            float2 vUV = _in.vUV + float2(0.f, vUVStep.y * (-6 + j));
            vColor += g_tex_0.Sample(g_sam_2, vUV) * CrossFilter[j];
        }
    }
    vColor /= Total;
    
    return vColor;
}


VS_OUT VS_EffectMerge(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_EffectMerge(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    
    float4 vEffect = g_tex_0.Sample(g_sam_0, _in.vUV);
    float4 vEffectBlur = g_tex_1.Sample(g_sam_0, _in.vUV);
    
    float4 vBloom = pow(abs(pow(vEffect, 2.2f)) + abs(pow(vEffectBlur, 2.2f)), 1.f / 2.2f);
    vBloom = saturate(vBloom);
    return vBloom;
}

#endif