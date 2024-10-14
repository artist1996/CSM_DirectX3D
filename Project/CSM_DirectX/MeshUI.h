#pragma once
#include "AssetUI.h"
class MeshUI :
    public AssetUI
{
private:

public:
    virtual void Update() override;

public:
    MeshUI();
    virtual ~MeshUI();
};

