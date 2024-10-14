#pragma once
#include "ComponentUI.h"
class TransformUI :
    public ComponentUI
{
private:

public:
    virtual void Update() override;

public:
    TransformUI();
    virtual ~TransformUI();
};