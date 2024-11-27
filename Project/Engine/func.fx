#ifndef _FUNC
#define _FUNC

#include "struct.fx"
#include "value.fx"

int IsBinding(Texture2D _tex)
{
    int width = 0; int height = 0;
    _tex.GetDimensions(width, height);
    
    if(0 == width || 0 == height)
        return false;
    
    return true;
}

void CalculateLight2D(int _LightIdx, float3 _WorldPos , inout tLight _Light)
{
    tLightInfo Info = g_Light2DBuffer[_LightIdx];
        
    if (0 == Info.Type)
    {
        _Light.Color.rgb += Info.light.Color.rgb;
        _Light.Ambient.rgb += Info.light.Ambient.rgb;
    }
    // Point Light
    else if (1 == Info.Type)
    {
        float fDist = distance(Info.WorldPos.xy, _WorldPos.xy);     // 두 거리의 차를 스칼라 값으로 반환시켜주는 함수
        //float Pow = saturate(1.f - (fDist / g_Light2DBuffer[0].Radius));
        float fPow = saturate(cos(saturate(fDist / Info.Radius) * (PI / 2.f)));
        
        _Light.Color.rgb += Info.light.Color.rgb * fPow;
        _Light.Ambient.rgb += Info.light.Ambient.rgb;
    }
    // Spot Light
    else
    {     
        float2 PixelDir = _WorldPos.xy - Info.WorldPos.xy;         // 픽셀의 월드포스와 광원의 월드포스 차이 벡터
        float fDist = length(PixelDir);                            // Radius 와 비교할 광원과 Pixel 의 Dist 값
        float2 Normalize = normalize(PixelDir);                    // 정규화 벡터로 만들어 준 후
        float Dot = saturate(dot(Info.WorldDir.xy, Normalize.xy)); // 두 방향 벡터의 내적 값인 cos 세타값을 계산해준다.
              
        if (Dot > cos(Info.Angle) && fDist < Info.Radius)
        {
            _Light.Color.rgb += Info.light.Color.rgb;
        }
    }
}

void CalculateLight3D(int _LightIdx, float3 _ViewNormal, float3 _ViewPos, inout tLight _Light)
{
    tLightInfo LightInfo = g_Light3DBuffer[_LightIdx];
    
    float LightPow    = 0.f;
    float SpecularPow = 0.f;
    float Ratio       = 1.f;
    float SpecRatio   = 1.f;
    
    if(0 == LightInfo.Type)
    {
        // VS 에서 받은 노말값으로, 빛의 세기를 PS 에서 직접 구한다음 빛의 세기를 적용   
        float3 vLightDir = normalize(mul(float4(LightInfo.WorldDir, 0.f), matView).xyz);
        LightPow = saturate(dot(-vLightDir, _ViewNormal));
            
        // 반사광 계산
        // vR = vL + 2 * dot(-vL, vN) * vN;
        float3 vReflect = vLightDir + 2 * dot(-vLightDir, _ViewNormal) * _ViewNormal;
        vReflect = normalize(vReflect);
        
        // 카메라에서 물체를 향하는 vEye 를 구한다. 카메라는 원점에 있다.
        // 픽셀의 뷰스페이스 위치가 곧 카메라에서 물체를 향하는 Eye 방향이다.
        float3 vEye = normalize(_ViewPos);
    
        // 반사 방향과 시선 벡터를 내적해서 둘 사이의 벌어진 각도에 대한 cos 값을 반사광의 세기로 사용한다.
        SpecularPow = saturate(dot(vReflect, -vEye));
        SpecularPow = pow(SpecularPow, 15);
    } 
    else if(1 == LightInfo.Type)
    {
        // 표면 위치에서 광원의 위치를 빼줘, 광원에서 표면을 향하는 방향 벡터를 구한다.
        float3 vLightViewPos = mul(float4(LightInfo.WorldPos, 1.f), matView).xyz;
        float3 vLightDir = normalize(_ViewPos - vLightViewPos);
        
        // 빛의 세기를 구해준다. 내적을 통해 알아야 하는데 스칼라 값이 음수 값이 나올 수 있으므로 0 ~ 1 값으로 정규화 해준다.
        LightPow = saturate(dot(-vLightDir, _ViewNormal));
        
        // 반사광을 구한다.
        // vR = vL + 2 * dot(-vL * vN) * vN;
        float3 vReflect = vLightDir + 2 * dot(-vLightDir, _ViewNormal) * _ViewNormal;
        vReflect = normalize(vReflect);

        // 카메라에서 물체를 향하는 vEye 를 구한다. 카메라는 원점에 있다.
        // 픽셀의 View Space 위치가 곧 카메라에서 물체를 향하는 Eye 방향이다.
        float3 vEye = normalize(_ViewPos);
        
        // 반사 방향과 시선 벡터를 내적해서 둘 사이의 벌어진 각도에 대한 cos 값을 반사광의 세기로 사용한다.
        SpecularPow = saturate(dot(vReflect, -vEye));
        SpecularPow = pow(SpecularPow, 15);
        
        // 거리에 따른 빛의 세기 감소량을 계산해준다.
        float Distance = length(vLightViewPos - _ViewPos);
        float CamDistance = length(_ViewPos);
        
        // 거리에 따른 빛의 세기 감소
        //Ratio = saturate(1.f - (Distance / LightInfo.Radius));
        // 거리에 따른 코사인 그래프의 영향을 받은 빛의 세기 감소
        Ratio     = saturate(cos((PI / 2.f) * saturate(Distance / LightInfo.Radius)));
        SpecRatio = saturate(cos((PI / 2.f) * saturate(CamDistance / LightInfo.Radius)));
    }
    
    //else if(2 == LightInfo.Type)
    //{
    //    // 표면 위치에서 광원의 위치를 빼줘, 광원에서 표면을 향하는 방향 벡터를 구한다.
    //    float3 vLightViewPos = mul(float4(LightInfo.WorldPos, 1.f), matView).xyz;
    //    float3 vLightDir = mul(float4(LightInfo.WorldDir, 0.f), matView).xyz;
    //    //float3 vLightDir = normalize(_ViewPos - vLightViewPos);
    //    
    //    // Pixel 의 View Position 과 Pixel 의 방향 벡터를 구해준다.
    //    float3 vPixelViewPos = vLightViewPos - _ViewPos;
    //    float3 vPixelDir     = normalize(vPixelViewPos);
    //    //float3 vPixelViewPos = _ViewPos - vLightViewPos;
    //    
    //    // Light 의 방향 벡터와 Pixel 의 방향벡터를 내적 해 cos 세타값을 구해준다.
    //    float Dot = saturate(dot(-vLightDir, vPixelDir));
    //            
    //    // Light의 ViewPos과 Pixel 의 View Postion 의 차를 구해 Distance 를 구해준다.
    //    float Distance = length(vLightViewPos - _ViewPos);
    //            
    //    // 내적한 cos 세타값이 Light의 Angle 안에 들어오고 Distance 가 Radius 내에 있다면
    //    // 빛의 세기를 구해준다.
    //    
    //    if (Dot > cos(LightInfo.Angle) && Distance < LightInfo.Radius * 2.f)
    //    {
    //        // 빛의 세기 계산
    //        LightPow = saturate(dot(-vLightDir, _ViewNormal));
    //        
    //        // 반사광 계산
    //        float3 vReflect = vLightDir + 2.f * dot(-vLightDir, _ViewNormal) * _ViewNormal;
    //        vReflect = normalize(vReflect);
    //        
    //        // 카메라에서 물체를 향하는 방향 계산      
    //        float3 vEye = normalize(_ViewPos);
    //      
    //        // 반사 방향과 시선 벡터의 내적 계산
    //        SpecularPow = saturate(dot(vReflect, -vEye));
    //        SpecularPow = pow(SpecularPow, 10);
    //        
    //        float fDist = length(vLightViewPos - _ViewPos);
    //        float fCamDist = length(_ViewPos);
    //        
    //        // 거리 비율 계산
    //        Ratio = saturate(cos((PI / 2.f) * saturate(fDist / LightInfo.Radius)));
    //        SpecRatio = saturate(cos((PI / 2.f) * saturate(fCamDist / LightInfo.Radius)));            
    //    }
    //}
    
    else if(2 == LightInfo.Type)
    {
        float3 vLightViewPos = mul(float4(LightInfo.WorldPos, 1.f), matView).xyz;
        float3 ToLight = vLightViewPos - _ViewPos;

        float fDistance = length(ToLight);
        
        if(LightInfo.Radius < fDistance)
            return;
        
        float3 vLightDir = mul(float4(LightInfo.WorldDir, 0.f), matView).xyz;
        
        float Dot = saturate(dot(-vLightDir, normalize(ToLight)));
        
        float LightTheta = cos(LightInfo.Angle);
        
        if (LightTheta < Dot)
        {
             // 빛의 세기 계산
            LightPow = saturate(dot(-vLightDir, _ViewNormal));
             
             // 반사광 계산
            float3 vReflect = vLightDir + 2.f * dot(-vLightDir, _ViewNormal) * _ViewNormal;
            vReflect = normalize(vReflect);
             
             // 카메라에서 물체를 향하는 방향 계산      
            float3 vEye = normalize(_ViewPos);
            
             // 반사 방향과 시선 벡터의 내적 계산
            SpecularPow = saturate(dot(vReflect, -vEye));
            SpecularPow = pow(SpecularPow, 10);
             
            float fDist = length(vLightViewPos - _ViewPos);
            float fCamDist = length(_ViewPos);
             
             // 거리 비율 계산
            Ratio = saturate(cos((PI / 2.f) * saturate(fDist / LightInfo.Radius)));
            SpecRatio = saturate(cos((PI / 2.f) * saturate(fCamDist / LightInfo.Radius)));
        }
    }
    
    _Light.Color += LightInfo.light.Color * LightPow * Ratio;
    _Light.Ambient += LightInfo.light.Ambient * Ratio;
    _Light.SpecCoef += LightInfo.light.SpecCoef * SpecularPow * SpecRatio;
}

float3 GetRandom(in Texture2D _NoiseTexture, uint _ID, uint _maxID)
{
    float2 vUV = (float2) 0.f;
    vUV.x = ((float) _ID / (float) (_maxID - 1) + g_EngineTime * 0.5f);
    vUV.y = sin(vUV.x * 20 * PI) * 0.5f + g_EngineTime * 0.1f;
    
    float3 vRandom = _NoiseTexture.SampleLevel(g_sam_1, vUV, 0).xyz;
    
    return vRandom;
}

float GetTessFactor(float _MinLevel, float _MaxLevel
                  , float _MinRange, float _MaxRange
                  , float3 _CamPos, float3 _Pos)
{
    float D = distance(_CamPos, _Pos);
    
    if(D < _MaxRange)
        return pow(2.f, _MaxLevel);
    else if (_MinRange < D)
        return pow(2.f, _MinLevel);
    else
    {
        float fRatio = 1.f - (D - _MaxRange) / abs(_MaxRange - _MinRange);
        float Level = 1.f + fRatio * (_MaxLevel - _MinLevel - 1.f);
        return pow(2.f, Level);
    }
}

int IntersectsRay(float3 _Pos[3], float3 _vStart, float3 _vDir, out float3 _CrossPos, out uint _Dist)
{
    // 삼각형 표면 방향 벡터
    float3 Edge[2] = { (float3) 0.f, (float3) 0.f };
    Edge[0] = _Pos[1] - _Pos[0];
    Edge[1] = _Pos[2] - _Pos[0];
    
    // 삼각형에 수직방향인 법선(Normal) 벡터
    float3 Normal = normalize(cross(Edge[0], Edge[1]));
    
    // 삼각형 법선벡터와 Ray 의 Dir 을 내적
    // 광선에서 삼각형으로 향하는 수직벡터와, 광선의 방향벡터 사이의 cos 값
    float NdotD = -dot(Normal, _vDir);
        
    float3 vStoP0 = _vStart - _Pos[0];
    float VerticalDist = dot(Normal, vStoP0); // 광선을 지나는 한점에서 삼각형 평면으로의 수직 길이
            
    // 광선이 진행하는 방향으로, 삼각형을 포함하는 평면까지의 거리
    float RtoTriDist = VerticalDist / NdotD;
        
    // 광선이, 삼각형을 포함하는 평면을 지나는 교점
    float3 vCrossPoint = _vStart + RtoTriDist * _vDir;
        
    // 교점이 삼각형 내부인지 테스트
    float3 P0toCross = vCrossPoint - _Pos[0];
    
    float3 Full = cross(Edge[0], Edge[1]);
    float3 U = cross(Edge[0], P0toCross);
    float3 V = cross(Edge[1], P0toCross);
       
    // 직선과 삼각형 평면의 교점이 삼각형 1번과 2번 사이에 존재하는지 체크
    //      0
    //     /  \
    //    1 -- 2    
    if (dot(U, Full) < 0.f)
        return 0;
    
    // 교점이 삼각형 내부인지 체크
    if (length(Full) < length(U) + length(V))
        return 0;
        
    _CrossPos = vCrossPoint;
    _Dist = (uint)RtoTriDist;
    
    return 1;
}

matrix GetBoneMat(int _iBoneIdx, int _iRowIdx)
{
    return g_arrBoneMat[(g_iBoneCount * _iRowIdx) + _iBoneIdx];
}

void Skinning(inout float3 _vPos, inout float3 _vTangent, inout float3 _vBinormal, inout float3 _vNormal
    , inout float4 _vWeight, inout float4 _vIndices
    , int _iRowIdx)
{
    tSkinningInfo info = (tSkinningInfo) 0.f;

    if (_iRowIdx == -1)
        return;

    for (int i = 0; i < 4; ++i)
    {
        if (0.f == _vWeight[i])
            continue;

        matrix matBone = GetBoneMat((int) _vIndices[i], _iRowIdx);

        info.vPos += (mul(float4(_vPos, 1.f), matBone) * _vWeight[i]).xyz;
        info.vTangent += (mul(float4(_vTangent, 0.f), matBone) * _vWeight[i]).xyz;
        info.vBinormal += (mul(float4(_vBinormal, 0.f), matBone) * _vWeight[i]).xyz;
        info.vNormal += (mul(float4(_vNormal, 0.f), matBone) * _vWeight[i]).xyz;
    }

    _vPos = info.vPos;
    _vTangent = normalize(info.vTangent);
    _vBinormal = normalize(info.vBinormal);
    _vNormal = normalize(info.vNormal);
}

#endif