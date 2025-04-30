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
    
    // 지지직
    //float2 vScreenUV = _in.vPosition.xy / g_Resolution;
    //
    //float2 vNoiseUV = vScreenUV;
    //
    //vNoiseUV.x = dot(vScreenUV.y + (g_EngineTime * 0.5f) * 10.f, vScreenUV.x + (g_EngineTime * 0.5f) * 10.f);
    //vNoiseUV.y = dot(vScreenUV.x + (g_EngineTime * 0.5f) * 10.f, vScreenUV.y + (g_EngineTime * 0.5f) * 10.f);

    //vNoiseUV.x = vScreenUV.x + sin(vScreenUV.y * 10.f + g_EngineTime) * 0.1f;
    //vNoiseUV.y = vScreenUV.y + cos(vScreenUV.x * 10.f + g_EngineTime) * 0.1f;
    
    // 불규칙 일렁임
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
    // 2. Pixel Shader Pixel 좌표
        
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
    
    // 지지직
    //vNoiseUV.x = dot(vScreenUV.y + (g_EngineTime * 0.5f) * 10.f, vScreenUV.x + (g_EngineTime * 0.5f) * 10.f);
    //vNoiseUV.y = dot(vScreenUV.x + (g_EngineTime * 0.5f) * 10.f, vScreenUV.y + (g_EngineTime * 0.5f) * 10.f);
    
    // 불규칙 일렁임  
    //vNoiseUV.x = vScreenUV.x + sin(vScreenUV.y * 10.f + g_EngineTime) * 0.1f;
    //vNoiseUV.y = vScreenUV.y + cos(vScreenUV.x * 10.f + g_EngineTime) * 0.1f;
      
    // 불꽃?
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
    
    // 풍덩
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
        
    //float2 vUVStep = 1.f / g_Resolution;
    //vUVStep *= 3.4f;
    //
    //if (_in.InstID == 0)
    //{
    //    for (int i = 0; i < 13; ++i)
    //    {
    //        float2 vUV = _in.vUV + float2(vUVStep.x * (-6 + i), 0.f);
    //        vColor += g_tex_0.Sample(g_sam_2, vUV) * CrossFilter[i];
    //    }
    //}
    //else if (_in.InstID == 1)
    //{
    //    for (int j = 0; j < 13; ++j)
    //    {
    //        float2 vUV = _in.vUV + float2(0.f, vUVStep.y * (-6 + j));
    //        vColor += g_tex_0.Sample(g_sam_2, vUV) * CrossFilter[j];
    //    }
    //}
    //vColor /= Total;
    
     // UV 스텝 크기 설정
    float2 vUVStep = 1.f / g_Resolution;
    vUVStep *= 1.f; // Gaussian 필터에서는 3배 확대 대신 1배로 설정
    
    float4 holdcolor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    // Gaussian 필터 적용 (5x5 필터)
    for (int i = -2; i <= 2; ++i)
    {
        for (int j = -2; j <= 2; ++j)
        {
        // 필터의 현재 위치에 따른 UV 좌표 계산
            float2 vUV = _in.vUV + float2(vUVStep.x * i, vUVStep.y * j);
        
        // 텍스처 샘플링 및 Gaussian 필터 값 적용
            vColor += g_tex_0.Sample(g_sam_2, vUV) * GaussianFilter[i + 2][j + 2];
        }
    }
    
    //vColor *= float4(2, 0, 0, 1.f);
    
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

VS_OUT VS_Threshold(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    //output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV;
    
    return output;
}


float4 PS_Threshold(VS_OUT _in) : SV_Target
{
    float3 color = pow(g_tex_0.Sample(g_sam_0, _in.vUV).rgb, 2.2f); // 감마 제거

    float luminance = dot(color, float3(0.2126, 0.7152, 0.0722)); // 밝기 추출

    float threshold = 0.9f;
    float3 bright = (luminance > threshold) ? color : float3(0.f, 0.f, 0.f);

    return float4(bright, 1.0);
}

VS_OUT VS_DownScale(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_DownScale(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.f, 0.f, 0.f, 1.f);

    float2 texelSize = 1.0f / g_Resolution;

    vOutColor += g_tex_0.Sample(g_sam_0, _in.vUV + float2(-texelSize.x, -texelSize.y)); // Top-left
    vOutColor += g_tex_0.Sample(g_sam_0, _in.vUV + float2(texelSize.x, -texelSize.y)); // Top-right
    vOutColor += g_tex_0.Sample(g_sam_0, _in.vUV + float2(-texelSize.x, texelSize.y)); // Bottom-left
    vOutColor += g_tex_0.Sample(g_sam_0, _in.vUV + float2(texelSize.x, texelSize.y)); // Bottom-right

    vOutColor *= 0.25f;

    return vOutColor;
}

VS_OUT VS_BloomBlur(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV       = _in.vUV;
    
    return output;
}

float4 PS_BloomBlur(VS_OUT _in) : SV_Target
{
   float4 vColor = float4(0.f, 0.f, 0.f, 0.f);

    // UV 스텝 크기 설정
    float2 vUVStep = 1.f / g_Resolution * 3.f;
    vUVStep *= 3.f;

    // Gaussian 필터 적용 (5x5 필터)
    for (int i = -2; i <= 2; ++i)
    {
        for (int j = -2; j <= 2; ++j)
        {
        // 필터의 현재 위치에 따른 UV 좌표 계산
            float2 vUV = _in.vUV + float2(vUVStep.x * i, vUVStep.y * j);
        
        // 텍스처 샘플링 및 Gaussian 필터 값 적용
            vColor += g_tex_0.Sample(g_sam_2, vUV) * GaussianFilter[i + 2][j + 2];
        }
    }
    
    return vColor;
}

VS_OUT VS_Bloom(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_Bloom(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    
    float4 vEffect = g_tex_0.Sample(g_sam_0, _in.vUV);
    float4 vEffectBlur = g_tex_1.Sample(g_sam_0, _in.vUV);
    
    float bloomIntensity = 0.3f; // 강도를 낮춰 부드럽게
    float4 finalColor = vEffect + vEffectBlur * bloomIntensity;
    return finalColor;
    
    float4 vBloom = pow(abs(pow(vEffect, 2.2f)) + abs(pow(vEffectBlur, 2.2f)), 1.f / 2.2f);
    vBloom = saturate(vBloom);
    return vBloom;
}

VS_OUT VS_FinalBloom(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_FinalBloom(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 0.f);
    
    float4 BlurTex = g_tex_0.Sample(g_sam_0, _in.vUV);
    float4 TargetTex = g_tex_1.Sample(g_sam_0, _in.vUV);
    
    float4 vBloom = pow(abs(pow(BlurTex, 2.2f)) + abs(pow(TargetTex, 2.2f)), 1.f / 2.2f);
    vBloom = saturate(vBloom);
    return vBloom;
}

#endif