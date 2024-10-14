#pragma once
#include "ComponentUI.h"
class Light3DUI :
    public ComponentUI
{
private:

public:
    virtual void Update() override;

public:
    Light3DUI();
    virtual ~Light3DUI();
};

