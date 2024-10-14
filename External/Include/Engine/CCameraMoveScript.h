#pragma once
#include "CScript.h"
class CCameraMoveScript :
    public CScript
{
private:
    float   m_CamSpeed;

private:
    void OrthoGraphicMove();
    void PerspectiveMove();

public:
    virtual void Tick() override;

public:
    CLONE(CCameraMoveScript);
    CCameraMoveScript();
    virtual ~CCameraMoveScript();
};

