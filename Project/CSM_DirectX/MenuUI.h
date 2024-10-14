#pragma once
#include "EditorUI.h"


class MenuUI :
    public EditorUI
{
private:

public:
    virtual void Tick() override;
    virtual void Update() override;

private:
    void File();
    void Level();
    void GameObject();
    void Assets();
    void Editor();
    
    void AddComponent();
    void AddScript();
    void CreateAnimation();

    void SaveLevel();
    void LoadLevel();

    wstring GetAssetKey(ASSET_TYPE _Type, const wstring& _strKey);
public:
    MenuUI();
    virtual ~MenuUI();
};

