#pragma once
#include "SE_Sub.h"

struct tSEInfo
{
    Vec2          LT;
    Vec2          Slice;
    Vec2          Offset;
    Vec2          BackGround;
};

class SE_Info :
    public SE_Sub
{
private:
    Ptr<CTexture>   m_Texture;
    tSEInfo         m_Info;

private:
    void Texture();
    void TextureInfo();
    void SpriteInfo();
    void CreateSprite();

private:
    void SetTexture(Ptr<CTexture> _Texture);
    void SelectTexture(DWORD_PTR _Param);
    void LoadTexture();

public:
    void SetInfo(tSEInfo _Info) { m_Info = _Info; }

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    virtual void Deactivate() override;

public:
    SE_Info();
    virtual ~SE_Info();
};

