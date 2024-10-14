#ifndef _TILEMAP
#define _TILEMAP

#include "value.fx"
#include "func.fx"

struct tTileInfo
{
    uint  ImgIdx;
    int3  Padding;
};

// =========================
// TileMap Shader Parameter
#define AtlasTex            g_tex_0
#define IsAtlasBind         g_btex_0
#define AtlasMaxRow         g_int_1
#define AtlasMaxCol         g_int_2
#define TileSliceUV         g_vec2_0
#define TileColRow          g_vec2_1
StructuredBuffer<tTileInfo> g_buffer : register(t15);

// =========================
struct VS_IN
{
    float3 vPos : POSITION;
    float2 vUV  : TEXCOORD;
};

struct VS_OUT
{
    float4 vPosition : SV_Position;
    float2 vUV       : TEXCOORD;
    float3 vWorldPos : Position;
};

VS_OUT VS_TileMap(VS_IN _in)
{
    VS_OUT output = (VS_OUT) 0.f;
    
    // TileMap 이 우측 하단으로 확장되도록 하기 위해서
    // Local Space의 RectMesh의 좌표를 수정한 후 상태행렬을 곱하게 한다.
    _in.vPos.x += 0.5f;
    _in.vPos.y -= 0.5f;
    
    output.vPosition = mul(float4(_in.vPos, 1.f), matWVP);
    output.vUV = _in.vUV * TileColRow;
    output.vWorldPos = mul(float4(_in.vPos, 1.f), matWorld).xyz;
    
    return output;
}

float4 PS_TileMap(VS_OUT _in) : SV_Target
{
    float4 vOutColor = (float4) 0.f;
        
    if(IsAtlasBind)
    {
        // LeftTop 계산
        // 픽셀쉐이더에서 본인의 타일이 몇번째 타일인지 알아내야한다.
        // 그 정보로 g_Buffer 에 전달된 각 타일정보중 본인의 정보에 접근해서 ImgIdx 를 알아낸다.
        // 알아낸 ImgIdx 로 LeftTopUV 값을 계산한다.
        
        // 입력으로 들어온 UV 값을 내림한 정수값만 취해준 값이 본인의 행 열이다.
        float2 CurColRow = floor(_in.vUV);
                      
        // 행 열을 1차원 Idx 로 변환 시켜줘야한다. 
        // 입력으로 들어온 현재 Tile 의 Col, Row 값의 열 값에 현재 내 행을 곱한 뒤 열을 더해준다.
        int Idx = TileColRow.x * CurColRow.y + CurColRow.x;
                
        int Row = g_buffer[Idx].ImgIdx / AtlasMaxCol;
        int Col = g_buffer[Idx].ImgIdx % AtlasMaxCol;
        
        float2 vLeftTopUV = float2(Col, Row) * TileSliceUV;
                
        // UV 좌표 계산
        float2 vUV = vLeftTopUV + frac(_in.vUV) * TileSliceUV;
        vOutColor = AtlasTex.Sample(g_sam_1, vUV);
        
        if(0.f == vOutColor.a)
            discard;
    }
    else
    {
        vOutColor = float4(1.f, 0.f, 1.f, 1.f);
    }
    
    tLight Light = (tLight) 0.f;
    
    for (int i = 0; i < g_Light2DCount; ++i)
    {
        CalculateLight2D(i, _in.vWorldPos, Light);
    }
    
    vOutColor.rgb = vOutColor.rgb * Light.Color.rgb 
                  + vOutColor.rgb * Light.Ambient.rgb;
    
    return vOutColor;
}

#endif