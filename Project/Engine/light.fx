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
    
    // Local ���� RectMesh �� 2��� Ű�� ����
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
    
    // Render Target ���� �ػ󵵴� ���� �����ϴ� UV ������ PositionTarget, NormalTarget �� ���� �� Position �� Normal ���� �����´�.
    float4 vViewPos     = POS_TARGET.Sample(g_sam_0, _in.vUV);
    
    // ViewPos �� w ���� 0 �̶��, �ƹ��͵� ���ٴ� ���̱� ������ discard ���ش�.
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
// Rasterizer   : CULL_FRONT(���� ���ο� ���� �� CULL_BACK ���� ���� �� �������� �ȵȴ�.), CULL_NONE ���� ���� �� ����ó���� 2��ø �Ǵ� ������ �ִ�.
// DepthStencil : NO_TEST_NO_WRITE
// BlendState   : ONE_ONE
// Parameter
// #define LIGHT_IDX       g_int_0
// #define POS_TARGET      g_tex_0
// #define NORMAL_TARGET   g_tex_1
// View �����, World ������� �ʿ�
// ViewPos �� ������� ���� World ����� ���� ����, Volume Mesh �� World ������� ���� Local Space �� ����
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
    
    // Pixel �� Position ���� UV ���� ���
    float2 vScreenUV = _in.vPosition.xy / g_Resolution;
    float4 vViewPos  = POS_TARGET.Sample(g_sam_0, vScreenUV);
    
    if (0.f == vViewPos.w)
        discard;

    // ViewPos �� View ����� ������� ���� WorldPos �� �����ش�.
    float3 vWorldPos = mul(float4(vViewPos.xyz, 1.f), matViewInv).xyz;
    
    // Local �� ���� �ؼ� Volume Mesh ���� ������ �ִ��� Ȯ���ؾ��Ѵ�.
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), matWorldInv).xyz;
    
    // Pixel �� Local Position �� Volume Mesh ���ο� �ִ��� Ȯ�� ��, ���ο� ���ٸ� discard ���ش�.
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
// View �����, World ������� �ʿ�
// ViewPos �� ������� ���� World ����� ���� ����, Volume Mesh �� World ������� ���� Local Space �� ����
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
    
    // Pixel �� Position ���� UV ���� ���
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