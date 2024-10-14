#ifndef _UI
#define _UI

#include "value.fx"
#include "struct.fx"

struct VTX_IN
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
    float3 vWorldPos : POSITION;
};

VTX_OUT VS_UI(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    //output.vPosition = mul(output.vPosition, matProj);
    output.vUV = _in.vUV;
    
    return output;
}

float4 PS_UI(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
    
    if (UseFlipBook)
    {
        // in.vUV : 스프라이트 를 참조할 위치를 비율로 환산한 값
        
        float2 BackGroundLeftTop = LeftTopUV - (BackGroundUV - SliceUV) / 2.f;
        float2 vSpriteUV = BackGroundLeftTop + (_in.vUV * BackGroundUV);

        vSpriteUV -= OffsetUV;
        
        
        if (LeftTopUV.x <= vSpriteUV.x && vSpriteUV.x <= LeftTopUV.x + SliceUV.x
            && LeftTopUV.y <= vSpriteUV.y && vSpriteUV.y <= LeftTopUV.y + SliceUV.y)
        {
            vColor = g_AtlasTex.Sample(g_sam_1, vSpriteUV);
        }
        else
        {
            discard;
        }
    }
    else
    {
        if (g_btex_0)
        {
            vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        }
        else
        {
            vColor = float4(0.f, 0.f, 0.f, 1.f);
        }
    }
    
    if (0.f == vColor.a)
        discard;
        
    return vColor;
}

float4 Player_HUD(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
    
    if (_in.vUV.y < 1.f - g_float_1)
        discard;
    
    if (g_btex_0)
    {
        vColor = g_tex_0.Sample(g_sam_1, float2(_in.vUV.x, _in.vUV.y));
    }
    else
    {
        vColor = float4(1.f, 0.f, 1.f, 1.f);
    }
    
    if (0.f == vColor.a)
        discard;
    
    return vColor;
}

float4 Player_FatigueHUD(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f,0.f,0.f,1.f);

    if (_in.vUV.x > g_float_2)
        discard;
    
    if (g_btex_0)
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    else
        vColor = float4(1.f, 0.f, 1.f, 1.f);

    if (0.f == vColor.a)
        discard;

    return vColor;
}

float4 PS_Icon(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);

    if (g_btex_0)
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);

    if (0.f == vColor.a)
        discard;

    if (_in.vUV.y < 1.f - g_float_1 && 0.f != g_float_1)
    {
        vColor.rgb *= vColor.rgb * 0.5f;
    }

    return vColor;
}

float4 PS_MonsterFace(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if(g_btex_0)
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    else
        vColor = float4(0.f, 0.f, 0.f, 1.f);
    
    if(0.f == vColor.a)
        discard;
    
    return vColor;
}

float4 PS_BossHUD(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (_in.vUV.x >= g_float_1)
        discard;
    
    if(g_btex_0)
        vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
    else
        vColor = float4(1.f, 0.f, 1.f, 1.f);
    
    if (0.f == vColor.a)
        discard;
    
    return vColor;
}

#endif