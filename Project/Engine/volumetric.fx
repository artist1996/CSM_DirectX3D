#ifndef _VOLUMETRIC
#define _VOLUMETRIC

#include "value.fx"
#include "struct.fx"

#define DT          g_EngineDT      // DeltaTime
#define TIME        g_EngineTime    // Engine Time

RWTexture3D<float4>                 g_OutputTex     : register(u0);
RWStructuredBuffer<tTestOutput>     g_OutputBuffer  : register(u1);
StructuredBuffer<tVolumetricModule> g_ModuleBuffer  : register(t21);

[numthreads(8,8,8)]
void CS_VolumetricLight(uint3 _ID : SV_DispatchThreadID)
{
    float3 TexSize = float3(g_ModuleBuffer[0].Width, g_ModuleBuffer[0].Height, g_ModuleBuffer[0].Depth);
    float3 vUVW = _ID / (TexSize - 1);
    
    g_OutputTex[_ID] = float4(1, 1, 0, 1);
    
    float3 vWorldPos = lerp(g_ModuleBuffer[0].vAABBMin.xyz, g_ModuleBuffer[0].vAABBMax.xyz, vUVW);
    
    g_OutputTex[_ID] = float4(1.f, 0.f, 0.f, 1.f);
    
    float fDist = distance(vWorldPos, g_ModuleBuffer[0].vLightWorldPos.xyz);
    
    if (fDist < g_ModuleBuffer[0].fRadius)
    {
        g_OutputTex[_ID] = float4(1, 1, 0, 1); // 노란색으로 테스트
        g_OutputBuffer[0].vColor = g_OutputTex[_ID].xyzw;
    }    
    
}


#endif