#ifndef _STD3D
#define _STD3D

#include "value.fx"
#include "func.fx"

struct VS_IN
{
    float3 vPos      : POSITION;
    float2 vUV       : TEXCOORD;
    
    float3 vTangent  : TANGENT;
    float3 vNormal   : NORMAL;
    float3 vBinormal : BINORMAL;
};

struct VS_OUT
{
    float4 vPosition     : SV_Position;
    float2 vUV           : TEXCOORD;
   
    float3 vViewPos      : POSITION;
    
    float3 vViewTangent  : TANGENT;
    float3 vViewNormal   : NORMAL;
    float3 vViewBinormal : BINORMAL;
};

static float3 g_LightDir = float3(1.f, -1.f, 1.f);
static float3 g_LightColor = float3(1.f, 1.f, 1.f);
static float3 g_Ambient = float3(0.15f, 0.15f, 0.15f);

// Gouraud Shading VS ���� ������ ���⸦ ���� ��, PS �� �ѱ�� ���
// Phong Shading  VS ���� Normal �� �ѱ��, PS ���� ������ ���⸦ ����ϴ� ���
VS_OUT VS_Std3D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV       = _in.vUV;
    
    
    output.vViewPos    = mul(float4(_in.vPos, 1.f), matWV).xyz;
    // View Space���� ���������� ǥ���� ��������
    output.vViewTangent  = normalize(mul(float4(_in.vTangent, 0.f), matWV).xyz);
    output.vViewNormal   = normalize(mul(float4(_in.vNormal, 0.f), matWV).xyz);
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), matWV).xyz);
    
    return output;
}

// �븻���� Ư¡
// 1. ������ ���� ���� 0 ~ 1 �� -1 ~ 1 �� ��ȯ�ؼ� ����ؾ� �Ѵ�.
// 2. Normal �ؽ�ó�ȿ� ����ִ� �븻���� ��ǥ �� ������ �ٸ���.
//  - xyz != xzy(rgb)

// 3. ��ǥ��
//  - �븻���� ������ ��ü������ �پ��� ����
//  - LocalSpace ���� �ٷ� ����� �� �ִ� Normal ���� ����Ǿ�����, ���밪 ����
//  - ���� - ������ ������.
//  - ���� - Ư�� �޽����� ���� ����
//         - ���� �����̽����� ����� �������� ���� �޽����� ������ �߰����� ��ġ�� �ʿ��ϴ�. 
//           EX) Animation ���� ��, �븻���͵� ���� �ִϸ��̼� ��ȯ�� ���� �����־�� ��

//  - �븻���� ������ ��ü������ Ǫ���� �迭
//  - Tangent(����) Space ��ǥ�� ������ �븻 ���� ����Ǿ�����, Relative ����
//  - ���� - ����� �� ��ĺ��� ���� �ɸ���.
//           TagentSpace �� �ִ� �븻���͸�, ���� ��ų ��ǥ��� ��ȯ�ϴ� ������ �ʿ��ϴ�.
//    ���� - ���������� ��� �����ϴ�.

float4 PS_Std3D(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.7f, 0.7f, 0.7f, 1.f);
    float3 vViewNormal = _in.vViewNormal;
        
    if (g_btex_0)
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    if (g_btex_1)
    {
        vViewNormal = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;
        // 1. ������ ���� ������ -1 ~ 1 �� ��ȯ�Ѵ�.
        vViewNormal = vViewNormal * 2.f - 1.f;
        
        // ǥ�� ��ǥ��(Tagent Space) ���� ������ ViewSpace �����Ű�� ���ؼ�
        // �����ų ���� ǥ�� ����(T, B, N) �� �ʿ��ϴ�.
        // ǥ�������� ȸ�� ����� �����Ѵ�. 
        float3x3 matRot =
        {
            _in.vViewTangent,
            _in.vViewBinormal,
            _in.vViewNormal,
        };
        
        vViewNormal = mul(vViewNormal, matRot);
    }
    
    tLight Light = (tLight)0.f;
   
    for (int i = 0; i < g_Light3DCount; ++i)
    {
        CalculateLight3D(i, vViewNormal, _in.vViewPos, Light);
    }
    
    if(g_int_3)
    {
        uint width, height;
        
        g_tex_0.GetDimensions(width, height);
        float2 offset = float2(1.f / float(width), 1.f / float(height));
        
        float4 leftcolor = g_AtlasTex.Sample(g_sam_1, float2(_in.vUV.x - offset.x, _in.vUV.y));
        float4 rightcolor = g_AtlasTex.Sample(g_sam_1, float2(_in.vUV.x + offset.x, _in.vUV.y));
        float4 upcolor = g_AtlasTex.Sample(g_sam_1, float2(_in.vUV.x, _in.vUV.y - offset.y));
        float4 downcolor = g_AtlasTex.Sample(g_sam_1, float2(_in.vUV.x, _in.vUV.y + offset.y));
    
        if (vOutColor.a != 0.f
                && 0.f == leftcolor.a
                || 0.f == rightcolor.a
                || 0.f == upcolor.a
                || 0.f == downcolor.a)
            vOutColor += g_vec4_1;
    }
    
    

    vOutColor.xyz = vOutColor.xyz * Light.Color.rgb
                  + vOutColor.xyz * Light.Ambient.rgb
                  + Light.Color.rgb * Light.SpecCoef;
    
    
    return vOutColor;
}

#endif