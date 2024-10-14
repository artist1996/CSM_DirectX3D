#pragma once
#include "TE_Sub.h"
class TE_Renderer :
    public TE_Sub
{
private:
    ImVec2   m_TilePos;

    ImVec2   m_uvMin;
    ImVec2   m_uvMax;

    int      m_TileIdx;
    int      m_TileImgIdx;

    float    m_WheelScale;
    float    m_Ratio;

    bool     m_UseGrid;

private:
    void WheelCheck();
    void RenderGrid();
    void RenderTileMap();

public:
    bool IsGrid()            { return m_UseGrid; }

    void UseGrid(bool _Use)  { m_UseGrid = _Use; }
    void SetImgIdx(int _Idx) { m_TileImgIdx = _Idx; }

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    TE_Renderer();
    virtual ~TE_Renderer();
};