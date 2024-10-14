#pragma once
#include "AssetUI.h"
class ComputeShaderUI :
    public AssetUI
{
private:

public:
    virtual void Update() override;

public:
    ComputeShaderUI();
    virtual ~ComputeShaderUI();
};

