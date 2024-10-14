#pragma once
#include "EditorUI.h"

class FileBrowser :
    public EditorUI
{
private:
    std::filesystem::path m_CurrentDirectory;
    std::filesystem::path m_strContentPath;
    Ptr<CTexture>         m_BrowserTex;
    Ptr<CTexture>         m_IconTex;

private:
    ASSET_TYPE FindAssetType(const std::filesystem::path& _Path);

public:
    virtual void Init()  override;
    virtual void Update() override;

public:
    FileBrowser();
    virtual ~FileBrowser();
};