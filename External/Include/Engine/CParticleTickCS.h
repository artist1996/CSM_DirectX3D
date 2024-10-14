#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CParticleTickCS :
    public CComputeShader
{
private:
    Ptr<CTexture>      m_NoiseTex;
    Ptr<CTexture>      m_Tex;

    CStructuredBuffer* m_ParticleBuffer;
    CStructuredBuffer* m_SpawnCountBuffer;
    CStructuredBuffer* m_ModuleBuffer;

    Vec3               m_ParticleWorldPos;

public:
    void SetNoiseTex(Ptr<CTexture> _Tex)                 { m_NoiseTex = _Tex; }

    void SetParticleBuffer(CStructuredBuffer* _Buffer)   { m_ParticleBuffer   = _Buffer; }
    void SetSpawnCountBuffer(CStructuredBuffer* _Buffer) { m_SpawnCountBuffer = _Buffer; }
    void SetModuleBuffer(CStructuredBuffer* _Buffer)     { m_ModuleBuffer     = _Buffer; }

    void SetParticleWorldPos(Vec3 _Pos)                  { m_ParticleWorldPos = _Pos; }

public:
    virtual int Binding()       override;
    virtual void CalcGroupNum() override;
    virtual void Clear()        override;

public:
    CParticleTickCS();
    virtual ~CParticleTickCS();
};

