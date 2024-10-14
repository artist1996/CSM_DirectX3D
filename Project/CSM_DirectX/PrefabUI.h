#pragma once
#include "AssetUI.h"
class PrefabUI :
    public AssetUI
{
private:
    int     m_Idx;

public:
    virtual void Update() override;

public:
    PrefabUI();
    virtual ~PrefabUI();
};

