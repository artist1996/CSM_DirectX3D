#ifndef _MERGE
#define _MERGE

#include "value.fx"

// ================================
// MergeShader
// MRT : SwapChain
// Mesh : RectMesh
// Rasterizer : CULL_BACK
// DepthStencil : NO_TEST_NO_WRITE
// BlendState   : Default
// Parameter
#define ALBEDO_TARGET   g_tex_0
#define DIFFUSE_TARGET  g_tex_1
#define SPECULAR_TARGET g_tex_2
#define EMISSIVE_TARGET g_tex_3
#define SHADOW_TARGET   g_tex_4
#define BLOOM_TARGET    g_tex_5

// ================================
// 기존 색상과 광원을 합쳐줘야한다.

#define USE_BLOOM       g_int_0

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

struct PS_OUT
{
    float4 vMerge  : SV_Target;
    float4 vBloom  : SV_Target1;
};

VS_OUT VS_Merge(VS_IN _in)
{
    VS_OUT output    = (VS_OUT) 0.f;
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV       = _in.vUV;
    
    return output;
};

float3 ToneMapping_Reinhard(float3 _vColor)
{
    return _vColor / (_vColor + 1.f);
}

PS_OUT PS_Merge(VS_OUT _in)
{
    PS_OUT output = (PS_OUT)0.f;
  

    float4 vColor    = ALBEDO_TARGET.Sample(g_sam_0, _in.vUV);
    float4 vDiffuse  = DIFFUSE_TARGET.Sample(g_sam_0, _in.vUV);
    float4 vSpecular = SPECULAR_TARGET.Sample(g_sam_0, _in.vUV);
    float4 vEmissive = EMISSIVE_TARGET.Sample(g_sam_0, _in.vUV);
    float4 vShadow   = SHADOW_TARGET.Sample(g_sam_3, _in.vUV);
    float4 vBloom    = BLOOM_TARGET.Sample(g_sam_1, _in.vUV);

    
    //float4 vLighting = (vDiffuse + vSpecular) * vShadow;
    //vLighting *= saturate(vShadow).r;
   
    output.vMerge = vColor * vDiffuse + vSpecular + vEmissive;
    float bloomIntensity = 1.2f;
    
    if (USE_BLOOM)
        output.vMerge += vBloom * 1.2f;
    
    // Tone Mapping
    //{
    //    float3 mergedColor = (vColor.rgb * vDiffuse.rgb + vSpecular.rgb + vEmissive.rgb);
    //
    //    if (USE_BLOOM)
    //        mergedColor += vBloom.rgb * 1.2f;
    //
    //    // Tone Mapping 적용
    //    mergedColor = ToneMapping_Reinhard(mergedColor);
    //
    //    // 감마 보정 (옵션)
    //    mergedColor = pow(mergedColor, 1.0f / 2.2f);
    //
    //    output.vMerge = float4(mergedColor, 1.0f);
    //}    

    return output;
};

#endif