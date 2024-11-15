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
// ================================
// 기존 색상과 광원을 합쳐줘야한다.

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

VS_OUT VS_Merge(VS_IN _in)
{
    VS_OUT output    = (VS_OUT) 0.f;
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV       = _in.vUV;
    
    return output;
};

static float CrossFilter[13] = { 0.0561f, 0.1353f, 0.278f, 0.4868f, 0.7261f, 0.9231f, 1.f, 0.9231f, 0.7261f, 0.4868f, 0.278f, 0.1353f, 0.0561f };
static float Total = 6.2108f;

void BlurShadow(inout float4 vColor, float2 UV)
{
    float4 Sum = float4(0.f, 0.f, 0.f, 0.f);
    float Offset = 0.0003f; // 블러 정도 조절
    
    Sum += SHADOW_TARGET.Sample(g_sam_0, UV + float2(Offset, 0)); // 오른쪽
    Sum += SHADOW_TARGET.Sample(g_sam_0, UV - float2(Offset, 0)); // 왼쪽
    Sum += SHADOW_TARGET.Sample(g_sam_0, UV + float2(0, Offset)); // 위
    Sum += SHADOW_TARGET.Sample(g_sam_0, UV - float2(0, Offset)); // 아래
    Sum += SHADOW_TARGET.Sample(g_sam_0, UV); // 현재 픽셀
    
    vColor = Sum / 5.f;
}

float4 PS_Merge(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;

    float4 vColor    = ALBEDO_TARGET.Sample(g_sam_0, _in.vUV);
    float4 vDiffuse  = DIFFUSE_TARGET.Sample(g_sam_0, _in.vUV);
    float4 vSpecular = SPECULAR_TARGET.Sample(g_sam_0, _in.vUV);
    float4 vEmissive = EMISSIVE_TARGET.Sample(g_sam_0, _in.vUV);
    float4 vShadow   = SHADOW_TARGET.Sample(g_sam_3, _in.vUV);
    
    //float4 vLighting = vDiffuse + vSpecular;
    //vLighting *= saturate(vShadow).r;
    //float4 vLighting = vDiffuse + vSpecular * saturate(vShadow).r;
    vDiffuse *= saturate(vShadow).r;
   
    //vOutColor = vColor * vLighting + vEmissive;
    vOutColor = vColor * vDiffuse + vSpecular + vEmissive;
    
    return vOutColor;
};

#endif