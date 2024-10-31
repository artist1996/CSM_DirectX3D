#pragma once
#include "ComponentUI.h"
class BoundingBoxUI :
    public ComponentUI
{
private:

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    BoundingBoxUI();
    ~BoundingBoxUI();
};

