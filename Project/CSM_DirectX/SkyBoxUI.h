#pragma once
#include "ComponentUI.h"
class SkyBoxUI :
    public ComponentUI
{
private:

public:
    virtual void Update() override;

public:
    SkyBoxUI();
    virtual ~SkyBoxUI();
};