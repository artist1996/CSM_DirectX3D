#pragma once
#include "EditorUI.h"
class CreateGameObject :
    public EditorUI
{
private:
    string m_strName;
    int    m_LayerIdx;

public:
    virtual void Deactivate() override;

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    CreateGameObject();
    virtual ~CreateGameObject();
};

