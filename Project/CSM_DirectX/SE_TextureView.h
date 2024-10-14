#pragma once
#include "SE_Sub.h"
class SE_TextureView :
    public SE_Sub
{
public:
    Ptr<CTexture> m_Texture;

    ImVec2        m_ImageRectMin; // Image 위젯 좌상단 Pos

    ImVec2        m_MousePos;     // 현재 Mouse Pos

    ImVec2        m_MouseLT;      // 마우스 위치에 해당하는 Texture 의 좌상단 픽셀 좌표
    ImVec2        m_MouseRB;      // 마우스 위치에 해당하는 Texture 의 우하단 픽셀 좌표

    set<Vec2>     m_PixelID;      // 등록된 적이 있는 픽셀인지 확인하는 용도

    float         m_WidthSize;
    float         m_WheelScale;   // Mouse Wheel
    float         m_Ratio;        // 실제 해상도 대비 출력 이미지의 비율

private:
    void WheelCheck();
    void SelectCheck();
    void DrawRect();
    void CalcSpriteSize(Vec2 _PixelPos);
    bool IsPixelOk(Vec2 _PixelPos);

    void SetInfo();

public:
    void SetTexture(Ptr<CTexture> _Texture);

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    virtual void Deactivate() override;

public:
    SE_TextureView();
    virtual ~SE_TextureView();
};

