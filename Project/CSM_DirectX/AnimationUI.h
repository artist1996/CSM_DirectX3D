#pragma once
#include "AssetUI.h"
class AnimationUI :
    public AssetUI
{
private:

public:
    virtual void Update() override;

private:
    virtual void SetAllInfo();

public:
    AnimationUI();
    virtual ~AnimationUI();
};

