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
        float fDist = distance(Info.WorldPos.xy, _WorldPos.xy);     // �� �Ÿ��� ���� ��Į�� ������ ��ȯ�����ִ� �Լ�
        //float Pow = saturate(1.f - (fDist / g_Light2DBuffer[0].Radius));
        float fPow = saturate(cos(saturate(fDist / Info.Radius) * (PI / 2.f)));
        
        _Light.Color.rgb += Info.light.Color.rgb * fPow;
        _Light.Ambient.rgb += Info.light.Ambient.rgb;
    }
    // Spot Light
    else
    {     
        float2 PixelDir = _WorldPos.xy - Info.WorldPos.xy;         // �ȼ��� ���������� ������ �������� ���� ����
        float fDist = length(PixelDir);                            // Radius �� ���� ������ Pixel �� Dist ��
        float2 Normalize = normalize(PixelDir);                    // ����ȭ ���ͷ� ����� �� ��
        float Dot = saturate(dot(Info.WorldDir.xy, Normalize.xy)); // �� ���� ������ ���� ���� cos ��Ÿ���� ������ش�.
              
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
        // VS ���� ���� �븻������, ���� ���⸦ PS ���� ���� ���Ѵ��� ���� ���⸦ ����   
        float3 vLightDir = normalize(mul(float4(LightInfo.WorldDir, 0.f), matView).xyz);
        LightPow = saturate(dot(-vLightDir, _ViewNormal));
            
        // �ݻ籤 ���
        // vR = vL + 2 * dot(-vL, vN) * vN;
        float3 vReflect = vLightDir + 2 * dot(-vLightDir, _ViewNormal) * _ViewNormal;
        vReflect = normalize(vReflect);
        
        // ī�޶󿡼� ��ü�� ���ϴ� vEye �� ���Ѵ�. ī�޶�� ������ �ִ�.
        // �ȼ��� �佺���̽� ��ġ�� �� ī�޶󿡼� ��ü�� ���ϴ� Eye �����̴�.
        float3 vEye = normalize(_ViewPos);
    
        // �ݻ� ����� �ü� ���͸� �����ؼ� �� ������ ������ ������ ���� cos ���� �ݻ籤�� ����� ����Ѵ�.
        SpecularPow = saturate(dot(vReflect, -vEye));
        SpecularPow = pow(SpecularPow, 15);
    } 
    else if(1 == LightInfo.Type)
    {
        // ǥ�� ��ġ���� ������ ��ġ�� ����, �������� ǥ���� ���ϴ� ���� ���͸� ���Ѵ�.
        float3 vLightViewPos = mul(float4(LightInfo.WorldPos, 1.f), matView).xyz;
        float3 vLightDir = normalize(_ViewPos - vLightViewPos);
        
        // ���� ���⸦ �����ش�. ������ ���� �˾ƾ� �ϴµ� ��Į�� ���� ���� ���� ���� �� �����Ƿ� 0 ~ 1 ������ ����ȭ ���ش�.
        LightPow = saturate(dot(-vLightDir, _ViewNormal));
        
        // �ݻ籤�� ���Ѵ�.
        // vR = vL + 2 * dot(-vL * vN) * vN;
        float3 vReflect = vLightDir + 2 * dot(-vLightDir, _ViewNormal) * _ViewNormal;
        vReflect = normalize(vReflect);

        // ī�޶󿡼� ��ü�� ���ϴ� vEye �� ���Ѵ�. ī�޶�� ������ �ִ�.
        // �ȼ��� View Space ��ġ�� �� ī�޶󿡼� ��ü�� ���ϴ� Eye �����̴�.
        float3 vEye = normalize(_ViewPos);
        
        // �ݻ� ����� �ü� ���͸� �����ؼ� �� ������ ������ ������ ���� cos ���� �ݻ籤�� ����� ����Ѵ�.
        SpecularPow = saturate(dot(vReflect, -vEye));
        SpecularPow = pow(SpecularPow, 15);
        
        // �Ÿ��� ���� ���� ���� ���ҷ��� ������ش�.
        float Distance = length(vLightViewPos - _ViewPos);
        float CamDistance = length(_ViewPos);
        
        // �Ÿ��� ���� ���� ���� ����
        //Ratio = saturate(1.f - (Distance / LightInfo.Radius));
        // �Ÿ��� ���� �ڻ��� �׷����� ������ ���� ���� ���� ����
        Ratio     = saturate(cos((PI / 2.f) * saturate(Distance / LightInfo.Radius)));
        SpecRatio = saturate(cos((PI / 2.f) * saturate(CamDistance / LightInfo.Radius)));
    }
    
    //else if(2 == LightInfo.Type)
    //{
    //    // ǥ�� ��ġ���� ������ ��ġ�� ����, �������� ǥ���� ���ϴ� ���� ���͸� ���Ѵ�.
    //    float3 vLightViewPos = mul(float4(LightInfo.WorldPos, 1.f), matView).xyz;
    //    float3 vLightDir = mul(float4(LightInfo.WorldDir, 0.f), matView).xyz;
    //    //float3 vLightDir = normalize(_ViewPos - vLightViewPos);
    //    
    //    // Pixel �� View Position �� Pixel �� ���� ���͸� �����ش�.
    //    float3 vPixelViewPos = vLightViewPos - _ViewPos;
    //    float3 vPixelDir     = normalize(vPixelViewPos);
    //    //float3 vPixelViewPos = _ViewPos - vLightViewPos;
    //    
    //    // Light �� ���� ���Ϳ� Pixel �� ���⺤�͸� ���� �� cos ��Ÿ���� �����ش�.
    //    float Dot = saturate(dot(-vLightDir, vPixelDir));
    //            
    //    // Light�� ViewPos�� Pixel �� View Postion �� ���� ���� Distance �� �����ش�.
    //    float Distance = length(vLightViewPos - _ViewPos);
    //            
    //    // ������ cos ��Ÿ���� Light�� Angle �ȿ� ������ Distance �� Radius ���� �ִٸ�
    //    // ���� ���⸦ �����ش�.
    //    
    //    if (Dot > cos(LightInfo.Angle) && Distance < LightInfo.Radius * 2.f)
    //    {
    //        // ���� ���� ���
    //        LightPow = saturate(dot(-vLightDir, _ViewNormal));
    //        
    //        // �ݻ籤 ���
    //        float3 vReflect = vLightDir + 2.f * dot(-vLightDir, _ViewNormal) * _ViewNormal;
    //        vReflect = normalize(vReflect);
    //        
    //        // ī�޶󿡼� ��ü�� ���ϴ� ���� ���      
    //        float3 vEye = normalize(_ViewPos);
    //      
    //        // �ݻ� ����� �ü� ������ ���� ���
    //        SpecularPow = saturate(dot(vReflect, -vEye));
    //        SpecularPow = pow(SpecularPow, 10);
    //        
    //        float fDist = length(vLightViewPos - _ViewPos);
    //        float fCamDist = length(_ViewPos);
    //        
    //        // �Ÿ� ���� ���
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
             // ���� ���� ���
            LightPow = saturate(dot(-vLightDir, _ViewNormal));
             
             // �ݻ籤 ���
            float3 vReflect = vLightDir + 2.f * dot(-vLightDir, _ViewNormal) * _ViewNormal;
            vReflect = normalize(vReflect);
             
             // ī�޶󿡼� ��ü�� ���ϴ� ���� ���      
            float3 vEye = normalize(_ViewPos);
            
             // �ݻ� ����� �ü� ������ ���� ���
            SpecularPow = saturate(dot(vReflect, -vEye));
            SpecularPow = pow(SpecularPow, 10);
             
            float fDist = length(vLightViewPos - _ViewPos);
            float fCamDist = length(_ViewPos);
             
             // �Ÿ� ���� ���
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
    // �ﰢ�� ǥ�� ���� ����
    float3 Edge[2] = { (float3) 0.f, (float3) 0.f };
    Edge[0] = _Pos[1] - _Pos[0];
    Edge[1] = _Pos[2] - _Pos[0];
    
    // �ﰢ���� ���������� ����(Normal) ����
    float3 Normal = normalize(cross(Edge[0], Edge[1]));
    
    // �ﰢ�� �������Ϳ� Ray �� Dir �� ����
    // �������� �ﰢ������ ���ϴ� �������Ϳ�, ������ ���⺤�� ������ cos ��
    float NdotD = -dot(Normal, _vDir);
        
    float3 vStoP0 = _vStart - _Pos[0];
    float VerticalDist = dot(Normal, vStoP0); // ������ ������ �������� �ﰢ�� ��������� ���� ����
            
    // ������ �����ϴ� ��������, �ﰢ���� �����ϴ� �������� �Ÿ�
    float RtoTriDist = VerticalDist / NdotD;
        
    // ������, �ﰢ���� �����ϴ� ����� ������ ����
    float3 vCrossPoint = _vStart + RtoTriDist * _vDir;
        
    // ������ �ﰢ�� �������� �׽�Ʈ
    float3 P0toCross = vCrossPoint - _Pos[0];
    
    float3 Full = cross(Edge[0], Edge[1]);
    float3 U = cross(Edge[0], P0toCross);
    float3 V = cross(Edge[1], P0toCross);
       
    // ������ �ﰢ�� ����� ������ �ﰢ�� 1���� 2�� ���̿� �����ϴ��� üũ
    //      0
    //     /  \
    //    1 -- 2    
    if (dot(U, Full) < 0.f)
        return 0;
    
    // ������ �ﰢ�� �������� üũ
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