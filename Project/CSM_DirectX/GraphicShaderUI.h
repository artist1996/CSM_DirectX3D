#pragma once
#include "AssetUI.h"
class GraphicShaderUI :
    public AssetUI
{
private:

public:
    virtual void Update() override;

public:
    GraphicShaderUI();
    virtual ~GraphicShaderUI();
};

