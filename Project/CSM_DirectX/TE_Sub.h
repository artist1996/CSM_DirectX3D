#pragma once
#include "EditorUI.h"

#include "TileMapEditor.h"

class TE_Sub :
    public EditorUI
{
private:
    TileMapEditor*     m_Owner;
    class CGameObject* m_TargetObject;
    
public:
    class TE_Detail* GetDetail()           { return m_Owner->GetDetail(); }
    class TE_TextureView* GetTextureView() { return m_Owner->GetTextureView(); }
    class TE_Renderer* GetRenderer()       { return m_Owner->GetRenderer(); }

public:
    void SetTargetObject(CGameObject* _Object) { m_TargetObject = _Object; }
    CGameObject* GetTargetObject()             { return m_TargetObject; }

public:
    virtual void Init() override;
    virtual void Update() override;

    virtual void Deactivate() override;

public:
    TE_Sub();
    virtual ~TE_Sub();

    friend class TileMapEditor;
};

