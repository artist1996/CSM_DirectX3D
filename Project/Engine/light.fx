#ifndef _LIGHT
#define _LIGHT

#include "value.fx"
#include "func.fx"

// ================================
// Directional Light Shader 
// MRT  : LIGHT (Diffuse, Specular)
// Mesh : RectMesh
// Rasterizer   : CULL_BACK
// DepthStencil : NO_TEST_NO_WRITE
// BlendState   : ONE_ONE
// Parameter
#define LIGHT_IDX       g_int_0
#define POS_TARGET      g_tex_0
#define NORMAL_TARGET   g_tex_1
// ================================

struct VS_DIR_IN
{
    float3 vPos : POSITION;
    float2 vUV  : TEXCOORD;
};

struct VS_DIR_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV       : TEXCOORD;
};

VS_DIR_OUT VS_DirLight(VS_DIR_IN _in)
{
    VS_DIR_OUT output = (VS_DIR_OUT) 0.f;
    
    // Local 에서 RectMesh 를 2배로 키워 적용
    output.vPosition = float4(_in.vPos * 2.f, 1.f);
    output.vUV       = _in.vUV;
    
    return output;
}

struct PS_OUT
{
    float4 vDiffuse  : SV_Target0;
    float4 vSpecular : SV_Target1;
};

PS_OUT PS_DirLight(VS_DIR_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // Render Target 들의 해상도는 전부 동일하니 UV 값으로 PositionTarget, NormalTarget 을 참조 해 Position 과 Normal 값을 가져온다.
    float4 vViewPos     = POS_TARGET.Sample(g_sam_0, _in.vUV);
    
    // ViewPos 의 w 값이 0 이라면, 아무것도 없다는 것이기 때문에 discard 해준다.
    if(0.f == vViewPos.w)
        discard;
    
    float3 vViewNormal  = NORMAL_TARGET.Sample(g_sam_0, _in.vUV).xyz;
    
    tLight light    = (tLight) 0.f;
    CalculateLight3D(LIGHT_IDX, vViewNormal, vViewPos.xyz, light);
    
    output.vDiffuse  = light.Color + light.Ambient;
    output.vSpecular = light.SpecCoef;
    
    output.vDiffuse.a  = 1.f;
    output.vSpecular.a = 1.f;
    
    return output;
}

#endif