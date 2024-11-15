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
#define SHADOWMAP       g_tex_2

#define LIGHT_VP        g_mat_0
// ================================

float3 ApplyFog(float3 _Color, float _Depth, float3 _FogColor, float _FogStart, float _FogEnd)
{
    float fogAmount = saturate((_Depth - _FogStart) / (_FogEnd - _FogStart));
    return lerp(_Color, _FogColor, fogAmount);
}

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
    float4 vDiffuse  : SV_Target;
    float4 vSpecular : SV_Target1;
    float4 vShadow   : SV_Target2;
};

PS_OUT PS_DirLight(VS_OUT _in)
{
    PS_OUT output = (PS_OUT) 0.f;
    
    float4 ShadowMapColor = 0.f;
    
    // Render Target ���� �ػ󵵴� ���� �����ϴ� UV ������ PositionTarget, NormalTarget �� ���� �� Position �� Normal ���� �����´�.
    float4 vViewPos     = POS_TARGET.Sample(g_sam_0, _in.vUV);
    
    // ViewPos �� w ���� 0 �̶��, �ƹ��͵� ���ٴ� ���̱� ������ discard ���ش�.
    if(0.f == vViewPos.w)
        discard;
    
    float2 vShadowMapUV = (float2) 0.f;
    int bShadow = false;
    //float2 vShadowMapUV = float2(0.f, 0.f);
    if(g_btex_2)
    {
        // ���� ���� ����(ViewPos) �� WorldPos �� �����ϰ�, ������������ ������Ų��.
        float3 vWorldPos = mul(float4(vViewPos.xyz, 1.f), matViewInv).xyz;
        float4 vProjPos = mul(float4(vWorldPos, 1.f), LIGHT_VP);
        vProjPos.xyz = vProjPos.xyz / vProjPos.w;
        
        // �������� ������Ų NDC ��ǥ�� UV �� ��ȯ�ؼ� ���������� ��ϵ� ��ü�� ���̸� Ȯ���Ѵ�.
        vShadowMapUV = float2((vProjPos.x + 1.f) * 0.5f, 1.f - (vProjPos.y + 1.f) * 0.5f);
        float fDist = g_tex_2.Sample(g_sam_3, vShadowMapUV).x;
        
        // ���� �������� ��ü�� ��ϵ� ����(�þ� ����) �̳������� �׽�Ʈ�� �����Ѵ�.
        if (0.f < vShadowMapUV.x && vShadowMapUV.x < 1.f
            && 0.f < vShadowMapUV.y && vShadowMapUV.y < 1.f)
        {
            // ������������ ��ϵ� ���̰���, ������ ���̸� ���Ѵ�.
            // ��ϵ� ���̺��� ���� ������Ų ���̰� �� ��ٸ�, ������������ ������ �����̴� ==> �׸��ڰ� ���ܾ� �Ѵ�.
            if (fDist + 0.0001f < vProjPos.z)
            {
                bShadow = true;
            }
        }
    }
    
    float3 vViewNormal  = NORMAL_TARGET.Sample(g_sam_0, _in.vUV).xyz;
    
    tLight light    = (tLight) 0.f;
    CalculateLight3D(LIGHT_IDX, vViewNormal, vViewPos.xyz, light);
    
    if(bShadow)
    {
        output.vShadow = float4(0.f, 0.f, 0.f, 1.f);
        //output.vDiffuse = light.Color + light.Ambient * 0.1f;
    }
    else
    {
        output.vShadow = float4(1.f, 1.f, 1.f, 1.f);
        output.vSpecular = light.SpecCoef;
        //output.vDiffuse = light.Color + light.Ambient;
    }
    
    //output.vDiffuse = float4(ApplyFog(output.vDiffuse.rgb, length(vViewPos.xyz), float3(0.f, 1.f, 0.f), 0.f, 10000.f), output.vDiffuse.a);

    output.vDiffuse = light.Color + light.Ambient;
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
    
    // �ȼ����̴��� ������ �������� Position ���� Ȯ���Ѵ�.    
    float2 vScreenUV = _in.vPosition.xy/*�ȼ� ��ǥ*/ / g_Resolution;
    float4 vViewPos = POS_TARGET.Sample(g_sam_0, vScreenUV);
    
    // ������ �ε��� ��ü�� �������� �ʴ´�.
    if (0.f == vViewPos.w)
    {
        discard;
    }
    
    // �ش� ��ü�� �����޽� �������� ����
    // ������ ��ü�� ViewPos �� WorldPos �� �����Ѵ�.
    float3 vWorldPos = mul(float4(vViewPos.xyz, 1.f), matViewInv).xyz;
    
    // World �� �ִ� ��ü�� ��ǥ��, Volume Mesh �� ���� ������� ���ؼ� Local �������� ��������.
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), matWorldInv).xyz;
    
    // ��ü�� �����޽� ���� ���̶�� ������� �ߴ�
    if (0.5f < length(vLocalPos))
    {
        discard;
    }
    
    float3 vViewNormal = NORMAL_TARGET.Sample(g_sam_0, vScreenUV).xyz;
    
    // �ش� ������ ���� ���� ���⸦ ����Ѵ�.
    tLight light = (tLight) 0.f;
    CalculateLight3D(LIGHT_IDX, vViewNormal, vViewPos.xyz, light);
        
    output.vDiffuse = light.Color + light.Ambient;
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
    float4 vViewPos = POS_TARGET.Sample(g_sam_0, vScreenUV);
       
    if (0.f == vViewPos.w)
        discard;
    
    // �ش� ��ü�� �����޽� �������� ����
    // ������ ��ü�� ViewPos �� WorldPos �� �����Ѵ�.
    float3 vWorldPos = mul(float4(vViewPos.xyz, 1.f), matViewInv).xyz;
    
    // World �� �ִ� ��ü�� ��ǥ��, Volume Mesh �� ���� ������� ���ؼ� Local �������� ��������.
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), matWorldInv).xyz;
    
    float Height = 1.f;
    float Radius = 0.5f;
    
    float Diagonal = sqrt(Height * Height + Radius * Radius);
    
    float CosTheta = Height / Diagonal;
    
    float3 Cone = float3(0.f,0.f, Height);
    
    float Dot = dot(normalize(vLocalPos), Cone);
      
    if(Dot < CosTheta)
        discard;
        
    tLight light = (tLight) 0.f;

    float3 vViewNormal = NORMAL_TARGET.Sample(g_sam_0, vScreenUV).xyz;

    CalculateLight3D(LIGHT_IDX, vViewNormal, vViewPos.xyz, light);
    
    output.vDiffuse = light.Color + light.Ambient;
    output.vSpecular = light.SpecCoef;
    output.vDiffuse.a = 1.f;
    output.vSpecular.a = 1.f;
        
    return output;
}

#endif