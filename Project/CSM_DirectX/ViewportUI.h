#pragma once
#include "EditorUI.h"
class ViewportUI :
    public EditorUI
{
private:

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    ViewportUI();
    ~ViewportUI();
};

