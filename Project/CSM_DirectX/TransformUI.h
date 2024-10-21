#pragma once
#include "ComponentUI.h"
class TransformUI :
    public ComponentUI
{
private:
    float m_Height;

public:
    virtual void Update() override;

public:
    TransformUI();
    virtual ~TransformUI();
};