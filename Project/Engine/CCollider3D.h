#pragma once
#include "CComponent.h"

enum class COLLIDER3D_TYPE
{
    CUBE,
    SPHERE,

    END,
};

class CCollider3D :
    public CComponent
{
private:
    Vec3    m_Offset;
    Vec3    m_Scale;       // 배율 or 절대 크기
    Matrix  m_matColWorld; // 충돌체의 최종 월드 상태
    int     m_OverlapCount;

    bool    m_IndependentScale; // 오브젝트의 크기에 영향을 받을지 여부
    bool    m_bRender;
    bool    m_bActive;

public:
    virtual void FinalTick() override;

public:
    void SetOffset(Vec3 _Offset)        { m_Offset = _Offset; }
    void SetScale(Vec3 _Scale)          { m_Scale = _Scale; }
    void SetIndependentScale(bool _Set) { m_IndependentScale = _Set; }

    Vec3 GetOffset()               { return m_Offset; }
    Vec3 GetScale()                { return m_Scale; }
    Vec3 GetWorldPos()             { return m_matColWorld.Translation(); }
    const Matrix& GetWorldMatrix() { return m_matColWorld; }

    int GetOverlapCount()    { return m_OverlapCount; }
    void ClearOverlapCount() { m_OverlapCount = 0; }
    void MinusOverlapCount() { m_OverlapCount -= 1; }

    bool IsIndependentScale() { return m_IndependentScale; }

    void SetRender(bool _Set) { m_bRender = _Set; }
    void SetActive(bool _Set) { m_bActive = _Set; }

    Vec3 GetHalfSize()
    {
        Vec3 scaleX = Vec3(m_matColWorld._11, m_matColWorld._12, m_matColWorld._13); // X축 벡터
        Vec3 scaleY = Vec3(m_matColWorld._21, m_matColWorld._22, m_matColWorld._23); // Y축 벡터
        Vec3 scaleZ = Vec3(m_matColWorld._31, m_matColWorld._32, m_matColWorld._33); // Z축 벡터

        return Vec3(scaleX.Length() * 0.5f, scaleY.Length() * 0.5f, scaleZ.Length() * 0.5f);
    }

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

public:
    void BeginOverlap(CCollider3D* _OtherCollider);
    void Overlap(CCollider3D* _OtherCollider);
    void EndOverlap(CCollider3D* _OtherCollider);

public:
    CLONE(CCollider3D);
    CCollider3D();
    CCollider3D(const CCollider3D& _Origin);
    virtual ~CCollider3D();
};

