#ifndef _LIGHT
#define _LIGHT

#include "value.fx"
#include "func.fx"

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

VS_OUT VS_DirLight(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
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

PS_OUT PS_DirLight(VS_OUT _in)
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

// ================================
// Point Light Shader 
// MRT  : LIGHT (Diffuse, Specular)
// Mesh : SphereMesh
// Rasterizer   : CULL_FRONT(광원 내부에 진입 시 CULL_BACK 으로 설정 시 렌더링이 안된다.), CULL_NONE 으로 설정 시 광원처리가 2중첩 되는 문제가 있다.
// DepthStencil : NO_TEST_NO_WRITE
// BlendState   : ONE_ONE
// Parameter
// #define LIGHT_IDX       g_int_0
// #define POS_TARGET      g_tex_0
// #define NORMAL_TARGET   g_tex_1
// View 역행렬, World 역행렬이 필요
// ViewPos 에 역행렬을 곱해 World 행렬을 구한 다음, Volume Mesh 에 World 역행렬을 곱해 Local Space 로 진입
// ================================

VS_OUT VS_PointLight(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV       = _in.vUV;
    
    return output;
}

PS_OUT PS_PointLight(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // Pixel 의 Position 으로 UV 값을 계산
    float2 vScreenUV = _in.vPosition.xy / g_Resolution;
    float4 vViewPos  = POS_TARGET.Sample(g_sam_0, vScreenUV);
    
    if (0.f == vViewPos.w)
        discard;

    // ViewPos 에 View 행렬의 역행렬을 곱해 WorldPos 를 구해준다.
    float3 vWorldPos = mul(float4(vViewPos.xyz, 1.f), matViewInv).xyz;
    
    // Local 로 접근 해서 Volume Mesh 내부 영역에 있는지 확인해야한다.
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), matWorldInv).xyz;
    
    // Pixel 의 Local Position 이 Volume Mesh 내부에 있는지 확인 후, 내부에 없다면 discard 해준다.
    if (0.5f < length(vLocalPos))
        discard;
  
    float3 vViewNormal = NORMAL_TARGET.Sample(g_sam_0, vScreenUV).xyz;
    
    tLight light = (tLight) 0.f;
    
    CalculateLight3D(LIGHT_IDX, vViewNormal, vViewPos.xyz, light);
    
    output.vDiffuse  = light.Color + light.Ambient;
    output.vSpecular = light.SpecCoef;
    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
    
    return output;
}

// ================================
// Spot Light Shader 
// MRT  : LIGHT (Diffuse, Specular)
// Mesh : ConeMesh
// Rasterizer   : CULL_BACK
// DepthStencil : NO_TEST_NO_WRITE
// BlendState   : ONE_ONE
// Parameter
// #define LIGHT_IDX       g_int_0
// #define POS_TARGET      g_tex_0
// #define NORMAL_TARGET   g_tex_1
// View 역행렬, World 역행렬이 필요
// ViewPos 에 역행렬을 곱해 World 행렬을 구한 다음, Volume Mesh 에 World 역행렬을 곱해 Local Space 로 진입
// ================================

VS_OUT VS_SpotLight(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV       = _in.vUV;
    
    return output;
}

PS_OUT PS_SpotLight(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    // Pixel 의 Position 으로 UV 값을 계산
    float2 vScreenUV = _in.vPosition.xy / g_Resolution;
    float4 vViewPos  = POS_TARGET.Sample(g_sam_0, vScreenUV);
    
    if (0.f == vViewPos.w)
        discard;

    float3 vViewNormal = NORMAL_TARGET.Sample(g_sam_0, vScreenUV).xyz;
    
    tLight light = (tLight) 0.f;
    
    CalculateLight3D(LIGHT_IDX, vViewNormal, vViewPos.xyz, light);
    
    output.vDiffuse = light.Color + light.Ambient;
    output.vSpecular = light.SpecCoef;
    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
    
    return output;
}

#endif