#pragma once
#include "ComponentUI.h"
class Light2DUI :
    public ComponentUI
{
private:

public:
    virtual void Update() override;

public:
    Light2DUI();
    virtual ~Light2DUI();
};

