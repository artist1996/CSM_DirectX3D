#ifndef _SETCOLOR
#define _SETCOLOR

#define TEX_WIDTH  g_int_0
#define TEX_HEIGHT g_int_1

#include "value.fx"

RWTexture2D<float4> g_TargetTex : register(u0);

// SV_GroupID         : 스레드가 속한 그룹의 ID 를 받는다.
// SV_GroupThreadID   : 속한 그룹에서의 스레드의 ID
// SV_GroupIndex      : SV_GroupThreadID 를 1차원 Index로 변환
// SV_DispathThreadID : 전체 기준 ID

// 그룹당 스레드 수 
[numthreads(32, 32, 1)]
void CS_SetColor(int3 _ID : SV_DispatchThreadID)
{
    // 픽셀을 초과해서 접근하는 스레드(초과 배정 스레드)
    if (_ID.x >= TEX_WIDTH || _ID.y >= TEX_HEIGHT)
        return;
     
    
    // 입력으로 들어온 _ID 값이 픽셀의 좌표
    g_TargetTex[_ID.xy] = g_vec4_0;

}

#endif