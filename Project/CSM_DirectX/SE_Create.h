#pragma once
#include "SE_Sub.h"


class SE_Create :
    public SE_Sub
{
private:
    Ptr<CTexture> m_Texture;
    Vec2          m_LT;
    Vec2          m_Slice;
    Vec2          m_Offset;
    Vec2          m_BackGround;

    string        m_strName;

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    void SetTexture(Ptr<CTexture> _Texture) { m_Texture = _Texture; }
    void SetInfo(Vec2 _LT, Vec2 _Slice, Vec2 _Offset, Vec2 _BackGround)
    {
        m_LT = _LT; 
        m_Slice = _Slice;
        m_Offset = _Offset;
        m_BackGround = _BackGround;
    }


public:
    virtual void Activate() override;
    virtual void Deactivate() override;

public:
    SE_Create();
    virtual ~SE_Create();
};

