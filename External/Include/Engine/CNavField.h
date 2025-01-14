#pragma once
#include "CComponent.h"

struct tBuildSetting
{
    int     iMaxCrowdNumber = 1024;    // 길찾기 주체들의 최대 개체수
    float   fMaxAgentRadius = 0.6f;    // 길찾기 주체들의 최대 충돌반경
    float   fSlopeAngle     = 45.f;    // 오를 수 있는 경사각의 최대치
    float   fClimb          = 0.2f;    // 오를 수 있는 단차의 최대치
    float   fMaxHeight      = 0.3f;    // 천장의 최소높이
    float   fDivisionSizeXZ = 0.3f;    // x, z 축의 공간 분할 단위
    float   fDivisionSizeY  = 0.2f;    // y 축의 공간 분할 단위
};

class CNavField :
    public CComponent
{
private:
    std::unique_ptr<rcContext> m_Context;
    rcPolyMesh*                m_PolyMesh;
    rcPolyMeshDetail*          m_PolyMeshDetail;
    rcConfig                   m_Config;

    dtNavMesh*                 m_NavMesh;
    dtNavMeshQuery*            m_NavQuery;
    dtCrowd*                   m_Crowd;

public:
    void BuildField(const float* worldVertices, size_t verticesNum, const int* faces, size_t facesNum, const tBuildSetting& buildSettings = tBuildSetting{});

public:
    virtual void FinalTick() override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

public:
    CLONE_DISABLE(CNavField);
    CNavField();
    ~CNavField();
};