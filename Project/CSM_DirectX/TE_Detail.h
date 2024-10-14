#pragma once
#include "TE_Sub.h"
class TE_Detail :
    public TE_Sub
{
private:
    Vec2 m_TileSize;
    int  m_MaxCol;
    int  m_MaxRow;

    int  m_CurIdx;

private:
    void TileMapInfo();

public:
    void SetCurrentIdx(int _Idx) { m_CurIdx = _Idx; }

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    TE_Detail();
    virtual ~TE_Detail();
};

