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
    float4 vDiffuse  : SV_TARGET;
    float4 vSpecular : SV_TARGET1;
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
    
    // 픽셀쉐이더랑 동일한 지점에서 Position 값을 확인한다.    
    float2 vScreenUV = _in.vPosition.xy/*픽셀 좌표*/ / g_Resolution;
    float4 vViewPos = POS_TARGET.Sample(g_sam_0, vScreenUV);
    
    // 광원이 부딪힐 물체가 존재하지 않는다.
    if (0.f == vViewPos.w)
    {
        discard;
    }
    
    // 해당 물체가 볼륨메쉬 내부인지 판정
    // 추출한 물체의 ViewPos 를 WorldPos 로 변경한다.
    float3 vWorldPos = mul(float4(vViewPos.xyz, 1.f), matViewInv).xyz;
    
    // World 상에 있는 물체의 좌표를, Volume Mesh 의 월드 역행렬을 곱해서 Local 공간으로 데려간다.
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), matWorldInv).xyz;
    
    // 물체가 볼륨메쉬 영역 밖이라면 광원계산 중단
    if (0.5f < length(vLocalPos))
    {
        discard;
    }
    
    float3 vViewNormal = NORMAL_TARGET.Sample(g_sam_0, vScreenUV).xyz;
    
    // 해당 지점이 받을 빛의 세기를 계산한다.
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
    float4 vViewPos = POS_TARGET.Sample(g_sam_0, vScreenUV);
    
    if (0.f == vViewPos.w)
        discard;
    
    // 해당 물체가 볼륨메쉬 내부인지 판정
    // 추출한 물체의 ViewPos 를 WorldPos 로 변경한다.
    float3 vWorldPos = mul(float4(vViewPos.xyz, 1.f), matViewInv).xyz;
    
    // World 상에 있는 물체의 좌표를, Volume Mesh 의 월드 역행렬을 곱해서 Local 공간으로 데려간다.
    float3 vLocalPos = mul(float4(vWorldPos, 1.f), matWorldInv).xyz;
           
    //float ConeVolume = ((1.f / 3.f) * PI) * pow(0.5f, 2) * 1.f;
    
    //if (vLocalPos.z <= 2 * sqrt(pow(vLocalPos.x, 2) + pow(vLocalPos.y, 2)))
    //    discard;
    
    //if (ConeVolume >= length(vLocalPos))
    //    discard;
    
    float Height = 1.f;
    float Radius = 0.5f;
    
    float Diagonal = sqrt(Height * Height + Radius * Radius);
    
    float CosTheta = Height / Diagonal;
    
    float3 Cone = float3(0.f,0.f, Height);
    
    float Dot = dot(normalize(vLocalPos), Cone);
    
    if(Dot < CosTheta)
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