#pragma once
#include "CRenderComponent.h"

#include "CParticleTickCS.h"

struct tSpawnCount
{
    UINT iSpawnCount;
    UINT Padding[3];
};

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    Ptr<CParticleTickCS> m_TickCS;

    CStructuredBuffer*   m_ParticleBuffer;      // 모든 파티클 정보
    CStructuredBuffer*   m_SpawnCountBuffer;    // 파티클 활성화 숫자 전달용 버퍼
    CStructuredBuffer*   m_ModuleBuffer;        // Module Data Buffer

    Ptr<CTexture>        m_ParticleTex;
    Ptr<CTexture>        m_EndTex;

    float                m_Time;                // 누적 시간
    float                m_BurstTime;           // SpawnBurst 체크용 변수

    int                  m_MaxParticleCount;    // 파티클 최대 개수

    tParticleModule      m_Module;              // 파티클의 기능 정의

    

public:
    void SetParticleTexture(Ptr<CTexture> _Texture) { m_ParticleTex = _Texture; }

    Ptr<CTexture>       GetParticleTex()      { return m_ParticleTex; }
    tParticleModule&    GetModuleInfo()       { return m_Module; }    
    int                 GetMaxParticleCount() { return m_MaxParticleCount; }
    
    UINT&               GetSpaceType()        { return m_Module.SpaceType; }

    void CreateModuleBuffer();

public:
    virtual void FinalTick() override;
    virtual void Render()    override;

    void CarculateSpawnCount();

    virtual void SaveToFile(FILE* _File)   override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CParticleSystem);
    CParticleSystem();
    CParticleSystem(const CParticleSystem& _Origin);
    virtual ~CParticleSystem();
};

