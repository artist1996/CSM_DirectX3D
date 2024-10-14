#pragma once
#include "ComponentUI.h"
class ParticleSystemUI :
    public ComponentUI
{
private:
    bool m_bChange;


public:
    virtual void Update() override;

public:
    ParticleSystemUI();
    virtual ~ParticleSystemUI();
};

