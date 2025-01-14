#pragma once
#include "CComponent.h"

class CBoundingSphere :
    public CComponent
{
private:
    float m_Radius;
    float m_Offset;

    bool  m_DebugRender;

public:
    virtual void FinalTick() override;

public:
    void SetOffset(float _Offset) { m_Offset = _Offset; }

    bool& IsDebugRender() { return m_DebugRender; }
    void SetDebugRender(bool _Render) { m_DebugRender = _Render; }

    float GetRadius() { return m_Radius; }
    float GetOffset() { return m_Offset; }

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

public:
    CLONE(CBoundingSphere);
    CBoundingSphere();
    ~CBoundingSphere();
};

