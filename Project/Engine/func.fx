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
    
    else if(2 == LightInfo.Type)
    {
        float3 vLightViewPos = mul(float4(LightInfo.WorldPos, 1.f), matView).xyz;
        float3 vLightDir = mul(float4(LightInfo.WorldDir, 0.f), matView).xyz;
        //float3 vLightViewPos = LightInfo.WorldPos;
        //float3 vLightDir = LightInfo.WorldDir;
        
        // Pixel �� View Position �� Pixel �� ���� ���͸� �����ش�.
        //float3 vPixelViewPos = LightInfo.WorldPos - _ViewPos;
        //float3 vPixelViewPos = _ViewPos - vLightViewPos;
        float3 vPixelViewPos = vLightViewPos - _ViewPos;

        float3 vPixelDir     = normalize(vPixelViewPos);
        
        // Light �� ���� ���Ϳ� Pixel �� ���⺤�͸� ���� �� cos ��Ÿ���� �����ش�.
        float Dot = saturate(dot(-vLightDir, vPixelDir));
        //float Dot = saturate(dot(vPixelDir, vLightDir));
        
        // Light�� ViewPos�� Pixel �� View Postion �� ���� ���� Distance �� �����ش�.
        float Distance = length(vPixelViewPos);
        
        // ������ cos ��Ÿ���� Light�� Angle �ȿ� ������ Distance �� Radius ���� �ִٸ�
        // ���� ���⸦ �����ش�.
        if (Dot > cos(LightInfo.Angle) && Distance < LightInfo.Radius)
        {          
            // ���� ���� ���
            LightPow = saturate(dot(-vLightDir, _ViewNormal));
            
            // �ݻ籤 ���
            float3 vReflect = vLightDir + 2.f * dot(-vLightDir, _ViewNormal) * _ViewNormal;
            vReflect = normalize(vReflect);
            
            // ī�޶󿡼� ��ü�� ���ϴ� ���� ���      
            float3 vEye = normalize(vLightViewPos - vPixelViewPos);
          
            // �ݻ� ����� �ü� ������ ���� ���
            SpecularPow = saturate(dot(vReflect, -vEye));
            SpecularPow = pow(SpecularPow, 20);
            
            // �Ÿ� ���� ���
            Ratio     = saturate(cos((PI / 2.f) * saturate(Distance / LightInfo.Radius)));
            SpecRatio = saturate(cos((PI / 2.f) * saturate(Distance / LightInfo.Radius)));
        }
    }
    
    _Light.Color    += LightInfo.light.Color * LightPow * Ratio;
    _Light.Ambient  += LightInfo.light.Ambient * Ratio;
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

#endif