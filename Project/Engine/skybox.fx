#ifndef _SKYBOX
#define _SKYBOX

#include "value.fx"

struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV  : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV       : TEXCOORD;
    float3 vLocalDir : TEXCOORD1;
};

VS_OUT VS_SkyBox(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    float4 vPos = mul(mul(float4(_in.vPos * 2.f, 0.f), matView), matProj);
    
    // z 값을 w 로 미리 세팅 해주어 z 값을 1로 고정시켜버린다.
    vPos.z = vPos.w;
    
    output.vPosition = vPos;
    output.vUV       = _in.vUV;
    output.vLocalDir = normalize(_in.vPos);
    
    //output.vUV.x += 1.f - cos(g_EngineTime * 0.01f);
    
    return output;
}

float4 PS_SkyBox(VS_OUT _in) : SV_Target
{
    float4 vColor = float4(0.4f, 0.4f, 0.4f, 1.f);
    
    if(0 == g_int_0)
    {
        if(g_btex_0)
            vColor = g_tex_0.Sample(g_sam_0, _in.vUV);
    }
    
    else if(1 == g_int_0)
    { 
        if(g_btexcube_0)
            vColor = g_texcube_0.Sample(g_sam_0, _in.vLocalDir);
    }
    
    return vColor;
}


#endif