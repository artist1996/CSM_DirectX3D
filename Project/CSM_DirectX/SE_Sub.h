#pragma once
#include "EditorUI.h"

#include "SpriteEditor.h"
#include <Engine/CAssetMgr.h>

class SE_Sub :
    public EditorUI
{
private:
    SpriteEditor* m_Owner;

public:
    SpriteEditor* GetOwner()               { return m_Owner; }
    class SE_Info* GetInfo()               { return m_Owner->GetInfo(); }
    class SE_TextureView* GetTextureView() { return m_Owner->GetTextureView(); }
    class SE_Create* GetCreate()           { return m_Owner->GetCreate(); }

public:
    SE_Sub();
    virtual ~SE_Sub();

    friend class SpriteEditor;
};

