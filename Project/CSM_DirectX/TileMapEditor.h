#pragma once
#include "EditorUI.h"

class TileMapEditor :
    public EditorUI
{
private:
    class TE_Detail*      m_Detail;
    class TE_TextureView* m_TextureView;
    class TE_Renderer*    m_Renderer;

public:
    TE_Detail* GetDetail()           { return m_Detail; }
    TE_TextureView* GetTextureView() { return m_TextureView; }
    TE_Renderer* GetRenderer()       { return m_Renderer; }

public:
    void SetTargetObject(CGameObject* _TargetObject);

public:
    virtual void Init() override;
    virtual void Update() override;
    
public:
    virtual void Activate() override;
    virtual void Deactivate() override;

public:
    TileMapEditor();
    virtual ~TileMapEditor();
};