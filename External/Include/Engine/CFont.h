#pragma once
#include "CComponent.h"

class CFont :
    public CComponent
{
private:
    wstring m_strFont;
    UINT    m_Color;

    float   m_Scale;

private:
    Vec3 CaculatePosition();

public:
    void SetColor(UINT _Color)            { m_Color = _Color; }
    void SetScale(float _Scale)           { m_Scale = _Scale; }
    void SetFont(const wstring& _strFont) { m_strFont = _strFont; }


public:
    virtual void FinalTick() override;

public:
    virtual void SaveToFile(FILE* _pFile)   override {}
    virtual void LoadFromFile(FILE* _pFile) override {}

public:
    CLONE(CFont);
    CFont();
    CFont(const CFont& _Other);
    virtual ~CFont();
};

