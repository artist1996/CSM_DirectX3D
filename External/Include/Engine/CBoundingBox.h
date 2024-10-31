#pragma once
#include "CComponent.h"

class CBoundingBox :
    public CComponent
{
private:
    float m_Radius;
    float m_Offset;

public:
    virtual void FinalTick() override;

public:
    void SetOffset(float _Offset) { m_Offset = _Offset; }

    float GetRadius() { return m_Radius; }
    float GetOffset() { return m_Offset; }

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

public:
    CLONE(CBoundingBox);
    CBoundingBox();
    ~CBoundingBox();
};