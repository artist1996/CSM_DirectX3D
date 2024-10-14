#pragma once
#include "ComponentUI.h"
class Collider2DUI :
    public ComponentUI
{
private:
    bool m_IS;

public:
    virtual void Update() override;

public:
    Collider2DUI();
    virtual ~Collider2DUI();
};

