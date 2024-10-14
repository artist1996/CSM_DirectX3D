#pragma once
#include "AE_Sub.h"
class AE_SpriteView :
    public AE_Sub
{
private:
    Ptr<CSprite> m_Sprite;

public:
    void SetSprite(Ptr<CSprite> _Sprite) { m_Sprite = _Sprite; }

public:
    virtual void Init() override;
    virtual void Update() override;

    virtual void Activate() override;
    virtual void Deactivate() override;

public:
    AE_SpriteView();
    virtual ~AE_SpriteView();
};

