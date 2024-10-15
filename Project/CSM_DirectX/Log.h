#pragma once
#include "EditorUI.h"

class Log :
    public EditorUI
{
private:
    UINT  m_FPS;
    UINT  m_PrevFPS;
    float m_FPSTime;

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    Log();
    virtual ~Log();
};

