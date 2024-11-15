#ifndef _TESS_TEST
#define _TESS_TEST

#include "value.fx"
// ================
// �׼����̼� �׽�Ʈ
// ================

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV  : TEXCOORD;
};

struct VS_OUT
{
    float3 vLocalPos : POSITION;
    float2 vUV       : TEXCOORD;
};

VS_OUT VS_Tess(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    output.vLocalPos = _in.vPos;
    output.vUV       = _in.vUV;
    
    return output;
}

// Hull Shader
// - Patch Constant Function (��ġ ��� �Լ�)
//   - ��ġ�� ȣ��Ǵ� �Լ�
// - Hull Shader Main �Լ�
//   - �� �������� ȣ��Ǵ� �Լ�

struct TessFactor
{
    float arrEdge[3] : SV_TessFactor;
    float Inside     : SV_InsideTessFactor;
};

TessFactor PatchConstantFunc(InputPatch<VS_OUT, 3> _in, uint _PatchIdx : SV_PrimitiveID)
{
    TessFactor output = (TessFactor) 0.f;
    
    output.arrEdge[0] = g_float_0;
    output.arrEdge[1] = g_float_0;
    output.arrEdge[2] = g_float_0;
    output.Inside     = g_float_0;
    
    return output;
}

struct HS_OUT
{
    float3 vLocalPos : POSITION;
    float2 vUV       : TEXCOORD;
};

[domain("tri")]
//[partitioning("integer")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("PatchConstantFunc")]
[maxtessfactor(64)]
HS_OUT HS_Tess(InputPatch<VS_OUT,3> _in
             , uint _PatchIdx : SV_PrimitiveID
             , uint _ControlPointID : SV_OutputControlPointID)
{
    HS_OUT output = (HS_OUT) 0.f;
        
    output.vLocalPos = _in[_ControlPointID].vLocalPos;
    output.vUV       = _in[_ControlPointID].vUV;
    return output;
}

// Domain Shader

struct DS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV       : TEXCOORD;
};

[domain("tri")]
DS_OUT DS_Tess(OutputPatch<HS_OUT, 3> _in
             , float3 Weight : SV_DomainLocation
             , TessFactor _PatchTessFactor)
{
    HS_OUT input = (HS_OUT) 0.f;
    
    for (int i = 0; i < 3; ++i)
    {
        input.vLocalPos += _in[i].vLocalPos * Weight[i];
        input.vUV       += _in[i].vUV * Weight[i];
    }
    
    DS_OUT output = (DS_OUT) 0.f;
    
    output.vPosition = mul(float4(input.vLocalPos, 1.f), matWVP);
    output.vUV       = input.vUV;
    
    return output;
}

float4 PS_Tess(DS_OUT _in) : SV_Target
{
    float4 vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    
    return vOutColor;
}

#endif