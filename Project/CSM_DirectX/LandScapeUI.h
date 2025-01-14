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
    void ChangeBrushIndex(DWORD_PTR _Param);

public:
    LandScapeUI();
    virtual ~LandScapeUI();
};