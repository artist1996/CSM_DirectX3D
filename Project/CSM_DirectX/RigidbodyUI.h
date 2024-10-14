#pragma once
#include "ComponentUI.h"
class RigidbodyUI :
    public ComponentUI
{
private:
    UINT m_UIHeight;

public:
    virtual void Update() override;

public:
    RigidbodyUI();
    virtual ~RigidbodyUI();
};

