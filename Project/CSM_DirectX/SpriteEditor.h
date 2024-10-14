#pragma once
#include "EditorUI.h"

class SpriteEditor :
    public EditorUI
{
private:
    class SE_TextureView* m_TextureView;
    class SE_Info*        m_SpriteInfo;
    class SE_Create*      m_Create;

public:
    SE_TextureView* GetTextureView() { return m_TextureView; }
    SE_Info* GetInfo()               { return m_SpriteInfo; }
    SE_Create* GetCreate()           { return m_Create; }

public:
    virtual void Init() override;
    virtual void Update() override;

    virtual void Activate() override;
    virtual void Deactivate() override;

public:
    SpriteEditor();
    virtual ~SpriteEditor();
};

