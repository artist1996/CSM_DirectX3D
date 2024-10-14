#pragma once
#include <Engine/CScript.h>

class CCameraMoveScript :
    public CScript
{
private:
    float        m_CamSpeed;

private:
    void OrthoGraphicMove();
    void PerspectiveMove();

public:
    virtual void Begin() override;
    virtual void Tick() override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

public:
    CLONE(CCameraMoveScript);
    CCameraMoveScript();
    virtual ~CCameraMoveScript();
};

