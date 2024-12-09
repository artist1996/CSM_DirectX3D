#pragma once
#include "ComponentUI.h"
class Animator3DUI :
    public ComponentUI
{
private:

public:
    virtual void Init()   override;
    virtual void Update() override;

public:
    Animator3DUI();
    ~Animator3DUI();
};

