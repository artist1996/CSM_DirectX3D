#pragma once
#include "EditorUI.h"
class AnimationEditor :
    public EditorUI
{
private:
    class AE_Preview*    m_Preview;
    class AE_Detail*     m_Detail;
    class AE_Create*     m_Create;
    class AE_SpriteView* m_SV;

private:
    void SaveAnimation();
    int LoadAnimation();

public:
    AE_Preview* GetPreview() { return m_Preview; }
    AE_Detail* GetDetail()   { return m_Detail; }
    AE_SpriteView* GetSV()   { return m_SV; }

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    virtual void Activate() override;
    virtual void Deactivate() override;

public:
    AnimationEditor();
    virtual ~AnimationEditor();
};

