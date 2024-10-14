#pragma once
#include "AssetUI.h"
class SpriteUI :
    public AssetUI
{
private:

public:
    virtual void Update() override;

public:
    SpriteUI();
    virtual ~SpriteUI();
};

