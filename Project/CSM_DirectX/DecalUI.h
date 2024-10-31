#pragma once
#include "ComponentUI.h"
class DecalUI :
    public ComponentUI
{
private:

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    DecalUI();
    ~DecalUI();
};

