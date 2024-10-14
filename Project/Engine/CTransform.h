#pragma once
#include "CComponent.h"

class CTransform :
    public CComponent
{
private:
    Vec3   m_RelativePos;
    Vec3   m_RelativeScale;
    Vec3   m_RelativeRotation;

    Vec3   m_RelativeDir[3];   // 방향
    Vec3   m_WorldDir[3];      // 최종 방향


    Matrix m_matWorld;         // 이동, 크기, 회전
    bool   m_IndependentScale; // 부모의 크기에 영향 받지 않음
    
public:
    void SetRelativePos(Vec3 _Pos)      { m_RelativePos = _Pos; }
    void SetRelativeScale(Vec3 _Scale)  { m_RelativeScale = _Scale; }
    void SetRelativeRotation(Vec3 _Rot) { m_RelativeRotation = _Rot; }
    
    void SetRelativePos(float x, float y, float z)      { m_RelativePos = Vec3(x, y, z); }
    void SetRelativeScale(float x, float y, float z)    { m_RelativeScale = Vec3(x, y, z); }
    void SetRelativeRotation(float x, float y, float z) { m_RelativeRotation = Vec3(x, y, z); }

    void SetWorldMatrix(const Matrix& _matWorld)        { m_matWorld = _matWorld; }
    void SetZAxis(float _z) { m_RelativePos.z = _z; }

    void SetIndependentScale(bool _Set)                 { m_IndependentScale = _Set; }

    Vec3 GetRelativePos()       { return m_RelativePos; }
    Vec3 GetRelativeScale()     { return m_RelativeScale; }
    Vec3 GetRelativeRotation()  { return m_RelativeRotation; }

    Vec3 GetRelativeDir(DIR _Dir) { return m_RelativeDir[_Dir]; };
    Vec3 GetWorldDir(DIR _Dir)    { return m_WorldDir[_Dir]; }
    Vec3 GetWorldPos()            { return m_matWorld.Translation(); }
    Vec3 GetWorldScale();

    bool IsIndependentScale()          { return m_IndependentScale; }

    const Matrix& GetWorldMatrix()     { return m_matWorld; }

public:
    virtual void FinalTick() override;
    void Binding();

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;
   
public:
    CLONE(CTransform)
    CTransform();
    virtual ~CTransform();
};

