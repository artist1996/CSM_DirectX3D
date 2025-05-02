#pragma once
#include "CComputeShader.h"

struct tVolumetricModule
{
	UINT Width;
	UINT Height;
	UINT Depth;

	float fRadius;				// 최대 거리
	float fAngle;				// 앵글
	float fScatteringIntensity;	// 산란 강도

	Vec3 vLightWorldPos;		// 라이트 월드 포지션
	Vec3 vLightWorldDir;		// 라이트 월드 방향 벡터
	Vec3 vLightCamWorldPos;		// 라이트 카메라 월드 포지션
	Vec3 vAABBMin;				// AABB Min
	Vec3 vAABBMax;				// AABB Max

	int padding[3];
};

struct tTestOutput
{
	Vec4 vColor;
};

class CStructuredBuffer;

class CVolumetricLightCS :
    public CComputeShader
{
private:
    Ptr<CTexture>			m_OutputTex;
	tVolumetricModule		m_Module;
	CStructuredBuffer*		m_ModuleBuffer;

	tTestOutput				m_Buffer;
	CStructuredBuffer*		m_OutputBuffer;

public:
    void SetOutputTex(Ptr<CTexture> _TargetTex) { m_OutputTex = _TargetTex; }

public:
    virtual int  Binding()      override;
    virtual void CalcGroupNum() override;
    virtual void Clear()        override;

	

public:
    CVolumetricLightCS();
    ~CVolumetricLightCS();
};