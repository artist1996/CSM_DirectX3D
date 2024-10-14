#ifndef _STD2D
#define _STD2D

#include "value.fx"
#include "func.fx"

// Vertex Shader
struct VTX_IN
{
    float3 vPos   : POSITION;
    float4 vColor : COLOR;
    float2 vUV    : TEXCOORD;
};

struct VTX_OUT
{
    float4 vPosition : SV_Position;
    float4 vColor    : COLOR;
    float2 vUV       : TEXCOORD;
    float3 vWorldPos : POSITION;    
};

VTX_OUT VS_Std2D(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    // LocalSpace -> WorldSpace
    // float 3 x float4x4(matrix)
    // float3 를 float4 로 차수를 맞추어준다.
    // 동차좌표를 1로 설정, 상태행렬 4행에 들어있는 이동을 적용받겠다는 뜻

    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vColor    = _in.vColor;
    output.vUV       = _in.vUV;
    
    output.vWorldPos = mul(float4(_in.vPos, 1.f), matWorld).xyz;
    
    return output;
}

float4 PS_Std2D(VTX_OUT _in) : SV_Target
{
    float4 vColor = float4(0.f, 0.f, 0.f, 1.f);
    
    float2 SpriteUV = float2(0.f, 0.f);
    // FlipBook 을 사용한다
    if (UseFlipBook)
    {
        // in.vUV : 스프라이트 를 참조할 위치를 비율로 환산한 값
        
        float2 BackGroundLeftTop = LeftTopUV - (BackGroundUV - SliceUV) / 2.f;
        float2 vSpriteUV = BackGroundLeftTop + (_in.vUV * BackGroundUV);
    
        vSpriteUV -= OffsetUV;       
        SpriteUV = vSpriteUV;
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
    // FlipBook 을 사용 하지 않는다.
    else
    {
        if (g_btex_0)
        {
            vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        }
        else
        {
            vColor = float4(1.f, 0.f, 1.f, 0.f);
        }
    } 
    
    if (vColor.a == 0.f)
    {
        discard;
    }
    
    if(g_int_2)
    {
        vColor *= float4(2.f, 1.f, 1.f, 1.f);
    }
    
    if (g_int_3)
    {
        uint width, height;
        
        g_AtlasTex.GetDimensions(width, height);
        float2 offset = float2(1.f / float(width), 1.f / float(height));
        
        float4 leftcolor = g_AtlasTex.Sample(g_sam_1, float2(SpriteUV.x - offset.x, SpriteUV.y));
        float4 rightcolor = g_AtlasTex.Sample(g_sam_1, float2(SpriteUV.x + offset.x, SpriteUV.y));
        float4 upcolor = g_AtlasTex.Sample(g_sam_1, float2(SpriteUV.x, SpriteUV.y - offset.y));
        float4 downcolor = g_AtlasTex.Sample(g_sam_1, float2(SpriteUV.x, SpriteUV.y + offset.y));
    
        if (vColor.a != 0.f
                && 0.f == leftcolor.a
                || 0.f == rightcolor.a
                || 0.f == upcolor.a
                || 0.f == downcolor.a)
            vColor += g_vec4_1;
    }
    
    //if(g_int_3)
    //{
    //    vColor = g_vec4_1;
    //}

    tLight Light = (tLight) 0.f;
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalculateLight2D(i, _in.vWorldPos, Light);
    }
    
    vColor.rgb = vColor.rgb * Light.Color.rgb 
               + vColor.rgb * Light.Ambient.rgb;
    
    return vColor;
}

float4 PS_Std2D_Alphablend(VTX_OUT _in) : SV_Target
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
    // FlipBook 을 사용 하지 않는다.
    else
    {
        if (g_btex_0)
        {
            vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        }
        else
        {
            discard;
        }
    }
    
    return vColor;
}

float4 PS_Less_AlphaBlend(VTX_OUT _in) : SV_Target
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
            vColor = float4(1.f, 0.f, 1.f, 1.f);
        }
    }
    
    return vColor;
}

float4 PS_Std2D_Additive(VTX_OUT _in) : SV_Target
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
    // FlipBook 을 사용 하지 않는다.
    else
    {
        if (g_btex_0)
        {
            vColor = g_tex_0.Sample(g_sam_1, _in.vUV);
        }
        else
        {
            vColor = float4(1.f, 0.f, 1.f, 1.f);
        }
    }
    
    
    if (0.03f >= vColor.r && 0.03f >= vColor.g && 0.03f >= vColor.b)
        discard;
    
    vColor.r *= 2.f;
    vColor.g *= 2.f;
    vColor.b *= 2.f;
     
    return vColor;
}

#endif