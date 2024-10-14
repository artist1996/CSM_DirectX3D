#pragma once
#include "CAsset.h"

class CSprite :
    public CAsset
{
private:
    Ptr<CTexture> m_Atlas; 
    Vec2          m_LeftTopUV;    // UV ÁÂÇ¥°è
    Vec2          m_SliceUV;      // UV ÁÂÇ¥°è
    Vec2          m_BackgroundUV; // UV ÁÂÇ¥°è
    Vec2          m_OffsetUV;     // UV ÁÂÇ¥°è

public:
    void Create(Ptr<CTexture> _Atlas, Vec2 _LeftTopPixel, Vec2 _SlicePixel);

public:
    Ptr<CTexture> GetAtlasTexture() { return m_Atlas; }
    void SetLeftTopUV(Vec2 _LeftTop);
    void SetSliceUV(Vec2 _Slice);
    void SetBackgroundUV(Vec2 _Background);
    void SetOffsetUV(Vec2 _Offset);

    Vec2 GetLeftTopUV()             { return m_LeftTopUV; }
    Vec2 GetSliceUV()               { return m_SliceUV; }
    Vec2 GetBackgroundUV()          { return m_BackgroundUV; }
    Vec2 GetOffsetUV()              { return m_OffsetUV; }

public:
    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _strRelativePath) override;

public:
    CSprite();
    virtual ~CSprite();
};

