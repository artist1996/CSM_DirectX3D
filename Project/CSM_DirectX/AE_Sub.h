#pragma once
#include "EditorUI.h"
#include "AnimationEditor.h"

class AE_Sub :
    public EditorUI
{
private:
    AnimationEditor* m_Owner;
    Ptr<CAnimation>  m_Animation;

public:
    class AE_Preview* GetPreview() { return m_Owner->GetPreview(); }
    class AE_Detail* GetDetail()   { return m_Owner->GetDetail(); }
    class AE_SpriteView* GetSV()   { return m_Owner->GetSV(); }

    void SetAnimation(Ptr<CAnimation> _Animation) { m_Animation = _Animation; }
    Ptr<CAnimation> GetAnimation()                { return m_Animation; }

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    AE_Sub();
    virtual ~AE_Sub();

    friend class AnimationEditor;
};

