#pragma once
#include "EditorUI.h"

class LevelEditor :
    public EditorUI
{
private:
    class CLevel*       m_CurLevel;

    string              m_strName;
    bool                m_CheckCollsionEditor;

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    virtual void Activate() override;
    virtual void Deactivate() override;

public:
    LevelEditor();
    virtual ~LevelEditor();
};

