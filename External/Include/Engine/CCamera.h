#pragma once
#include "CComponent.h"

enum PROJ_TYPE
{
    ORTHOGRAPHIC,       // 직교 투영
    PERSPECTIVE,        // 원근 투영
};

class CCamera :
    public CComponent
{
private:
    int       m_Priority;

    UINT      m_LayerCheck;      // 원하는 레이어만 카메라에 찍히도록 설정

    PROJ_TYPE m_ProjType;

    float     m_Width;
    float     m_Height;
    float     m_Far;             // 카메라가 바라볼 수 있는 z축의 거리
    float     m_AspectRatio;     // 종횡 비
    float     m_ProjectionScale;  

    float     m_FOV;             // Field Of View (시야 범위, 시야 각) 원근 투영 시

    Matrix    m_matView;
    Matrix    m_matProj;

    vector<CGameObject*> m_vecOpaque;       // 불투명
    vector<CGameObject*> m_vecMasked;       // 투명, 불투명
    vector<CGameObject*> m_vecTransparent;  // 투명, 반투명
    vector<CGameObject*> m_vecParticles;    // 투명, 반투명, 입자
    vector<CGameObject*> m_vecPostProcess;  // 후처리
    vector<CGameObject*> m_vecUI;           // UI

    bool                 m_ZoomIn;
    bool                 m_ZoomOut;

    bool                 m_ShakingIn;
    bool                 m_ShakingOut;

    bool                 m_UI;

public:
    void SetPriority(int _Priority) { m_Priority = _Priority; }
    void SetLayer(UINT _LayerIdx, bool _bCheck)
    {
        if (_bCheck)
            m_LayerCheck |= (1 << _LayerIdx);

        else
            m_LayerCheck &= ~(1 << _LayerIdx);
    }

    void SetLayerAll()                { m_LayerCheck = 0xffffffff; }
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    void SetWidth(float _Width)       { m_Width = _Width; m_AspectRatio = m_Width / m_Height; }
    void SetHeight(float _Height)     { m_Height = _Height; m_AspectRatio = m_Width / m_Height; }
    void SetFar(float _Far)           { m_Far = _Far; }
    void SetFOV(float _FOV)           { m_FOV = _FOV; }
    void SetProjScale(float _Scale)   { m_ProjectionScale = _Scale; }
    void SetUI(bool _Set)   { m_UI = _Set; }

    PROJ_TYPE GetProjType() { return m_ProjType; }
    float GetWidth()        { return m_Width; }
    float GetHeight()       { return m_Height; }
    float GetFar()          { return m_Far; }
    float GetFOV()          { return m_FOV; }
    float GetProjScale()    { return m_ProjectionScale; }
    float GetAspectRatio()  { return m_AspectRatio; }
    
    bool GetLayerCheck(int _LayerIdx) { return m_LayerCheck & (1 << _LayerIdx); }

    const Matrix& GetViewMatrix() { return m_matView; }
    const Matrix& GetProjMatrix() { return m_matProj; }

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

public:
    virtual void Begin() override;
    virtual void FinalTick() override;

private:
    void SortGameObject();

public:
    void Render();

public:
    CLONE(CCamera);
    CCamera();
    virtual ~CCamera();
};

