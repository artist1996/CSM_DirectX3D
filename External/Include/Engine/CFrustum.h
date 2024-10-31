#pragma once
#include "CEntity.h"

enum class FACE_TYPE
{
    FT_NEAR,
    FT_FAR,
    FT_TOP,
    FT_BOT,
    FT_LEFT,
    FT_RIGHT,

    END,
};

class CFrustum :
    public CEntity
{
private:
    class CCamera* m_Owner;
    Vec3           m_arrProj[8];                        // NDC 좌표계에서 시야의 최대위치 좌표
    Vec4           m_arrFace[(UINT)FACE_TYPE::END];  // 절두체를 구성하는 6개의 평면(월드 스페이스 기준)

public:
    void FinalTick();
    bool FrustumCheck(Vec3 _WorldPos, float _Radius = 0.f);
    
public:
    CLONE(CFrustum);
    CFrustum(CCamera* _Owner);
    ~CFrustum();
};