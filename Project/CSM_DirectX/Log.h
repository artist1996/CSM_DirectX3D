#pragma once
#include "EditorUI.h"

class Log :
    public EditorUI
{
private:

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    Log();
    virtual ~Log();
};

