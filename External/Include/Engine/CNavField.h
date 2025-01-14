#pragma once
#include "CComponent.h"

struct tBuildSetting
{
    int     iMaxCrowdNumber = 1024;    // ��ã�� ��ü���� �ִ� ��ü��
    float   fMaxAgentRadius = 0.6f;    // ��ã�� ��ü���� �ִ� �浹�ݰ�
    float   fSlopeAngle     = 45.f;    // ���� �� �ִ� ��簢�� �ִ�ġ
    float   fClimb          = 0.2f;    // ���� �� �ִ� ������ �ִ�ġ
    float   fMaxHeight      = 0.3f;    // õ���� �ּҳ���
    float   fDivisionSizeXZ = 0.3f;    // x, z ���� ���� ���� ����
    float   fDivisionSizeY  = 0.2f;    // y ���� ���� ���� ����
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