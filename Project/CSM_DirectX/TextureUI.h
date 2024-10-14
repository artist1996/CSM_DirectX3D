#pragma once
#include "AssetUI.h"
class TextureUI :
    public AssetUI
{
private:

public:
    virtual void Update() override;

public:
    TextureUI();
    virtual ~TextureUI();
};

