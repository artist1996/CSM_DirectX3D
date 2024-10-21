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

// Gouraud Shading VS 에서 광원의 세기를 구한 뒤, PS 로 넘기는 방식
// Phong Shading  VS 에서 Normal 을 넘기고, PS 에서 광원의 세기를 계산하는 방식
VS_OUT VS_Std3D(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
        
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV       = _in.vUV;
    
    
    output.vViewPos    = mul(float4(_in.vPos, 1.f), matWV).xyz;
    // View Space에서 정점에서의 표면의 수직방향
    output.vViewTangent  = normalize(mul(float4(_in.vTangent, 0.f), matWV).xyz);
    output.vViewNormal   = normalize(mul(float4(_in.vNormal, 0.f), matWV).xyz);
    output.vViewBinormal = normalize(mul(float4(_in.vBinormal, 0.f), matWV).xyz);
    
    return output;
}

// 노말맵핑 특징
// 1. 추출한 값의 범위 0 ~ 1 를 -1 ~ 1 로 변환해서 사용해야 한다.
// 2. Normal 텍스처안에 들어있는 노말값의 좌표 축 기준이 다르다.
//  - xyz != xzy(rgb)

// 3. 좌표계
//  - 노말맵의 색감이 전체적으로 다양한 색상
//  - LocalSpace 에서 바로 사용할 수 있는 Normal 값이 저장되어있음, 절대값 개념
//  - 장점 - 연산이 빠르다.
//  - 단점 - 특정 메쉬에만 적용 가능
//         - 로컬 스페이스에서 모양이 정해지지 않은 메쉬에는 적용이 추가적인 조치가 필요하다. 
//           EX) Animation 적용 시, 노말벡터도 같이 애니메이션 변환을 적용 시켜주어야 함

//  - 노말맵의 색감이 전체적으로 푸른색 계열
//  - Tangent(접선) Space 좌표계 기준의 노말 값이 저장되어있음, Relative 개념
//  - 단점 - 계산이 위 방식보다 좀더 걸린다.
//           TagentSpace 에 있는 노말벡터를, 적용 시킬 좌표계로 변환하는 과정이 필요하다.
//    장점 - 범용적으로 사용 가능하다.

float4 PS_Std3D(VS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(0.7f, 0.7f, 0.7f, 1.f);
    float3 vViewNormal = _in.vViewNormal;
        
    if (g_btex_0)
        vOutColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    
    if (g_btex_1)
    {
        vViewNormal = g_tex_1.Sample(g_sam_0, _in.vUV).xyz;
        // 1. 추출한 값의 범위를 -1 ~ 1 로 변환한다.
        vViewNormal = vViewNormal * 2.f - 1.f;
        
        // 표면 좌표계(Tagent Space) 기준 방향을 ViewSpace 적용시키기 위해서
        // 적용시킬 곳의 표면 정보(T, B, N) 이 필요하다.
        // 표면정보로 회전 행렬을 구성한다. 
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