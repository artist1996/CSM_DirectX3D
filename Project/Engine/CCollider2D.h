#pragma once
#include "CComponent.h"
class CCollider2D :
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

    int GetOverlapCount()     { return m_OverlapCount; }
    void ClearOverlapCount()  { m_OverlapCount = 0; }
    void MinusOverlapCount()  { m_OverlapCount -= 1; }

    bool IsIndependentScale() { return m_IndependentScale; }

    void SetRender(bool _Set) { m_bRender = _Set; }
    void SetActive(bool _Set) { m_bActive = _Set; }

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

public:
    void BeginOverlap(CCollider2D* _OtherCollider);
    void Overlap(CCollider2D* _OtherCollider);
    void EndOverlap(CCollider2D* _OtherCollider);

public:
    CLONE(CCollider2D);
    CCollider2D();
    CCollider2D(const CCollider2D& _Origin);
    virtual ~CCollider2D();
};

