#pragma once

#include "EditorUI.h"

class CollisionMatrix :
    public EditorUI
{
    
private:
    UINT m_MaxLength;

public:
    virtual void Init() override;
    virtual void Update() override;
    
public:
    CollisionMatrix();
    virtual ~CollisionMatrix();
};

