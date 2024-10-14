#pragma once
#include "TE_Sub.h"
class TE_TextureView :
    public TE_Sub
{
private:
    ImVec2  m_ImageRectMin;
    ImVec2  m_vDiff;

    float   m_WheelScale;
    float   m_Ratio;
    float   m_WidthSize;

    int     m_ClickCol;
    int     m_ClickRow;

private:
    void WheelCheck();
    void ClickedCheck();
    void DrawRect();

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    TE_TextureView();
    virtual ~TE_TextureView();
};

