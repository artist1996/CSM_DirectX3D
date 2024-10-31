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
    Vec3           m_arrProj[8];                        // NDC ��ǥ�迡�� �þ��� �ִ���ġ ��ǥ
    Vec4           m_arrFace[(UINT)FACE_TYPE::END];  // ����ü�� �����ϴ� 6���� ���(���� �����̽� ����)

public:
    void FinalTick();
    bool FrustumCheck(Vec3 _WorldPos, float _Radius = 0.f);
    
public:
    CLONE(CFrustum);
    CFrustum(CCamera* _Owner);
    ~CFrustum();
};