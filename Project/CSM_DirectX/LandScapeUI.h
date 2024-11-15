#pragma once
#include "ComponentUI.h"
class LandScapeUI :
    public ComponentUI
{
private:

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    LandScapeUI();
    ~LandScapeUI();
};

