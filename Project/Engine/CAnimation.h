#pragma once
#include "CAsset.h"

#include "CSprite.h"

class CAnimation :
    public CAsset
{
private:
    vector<Ptr<CSprite>> m_vecSprite;

public:
    void AddSprite(Ptr<CSprite> _Sprite)     { m_vecSprite.push_back(_Sprite); }
    void Insert(int _Idx, Ptr<CSprite> _InsertSprite);
    Ptr<CSprite> GetSprite(int _Idx)         { return m_vecSprite[_Idx]; }
    int GetMaxFrameCount()                   { return (int)m_vecSprite.size(); }
    const vector<Ptr<CSprite>>& GetSprites() { return m_vecSprite; }

    void erase(int _Idx);

public:
    virtual int Load(const wstring& _FilePath);
    virtual int Save(const wstring& _FilePath);


public:
    CAnimation();
    virtual ~CAnimation();
};