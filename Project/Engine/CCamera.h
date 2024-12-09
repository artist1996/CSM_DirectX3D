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
    int                  m_Priority;

    UINT                 m_LayerCheck;      // 원하는 레이어만 카메라에 찍히도록 설정

    PROJ_TYPE            m_ProjType;

    class CFrustum*      m_Frustum;         // 카메라는 절두체를 가진다.
    
    float                m_Width;
    float                m_Height;
    float                m_Far;             // 카메라가 바라볼 수 있는 z축의 거리
    float                m_AspectRatio;     // 종횡 비
    float                m_ProjectionScale;  
                         
    float                m_FOV;             // Field Of View (시야 범위, 시야 각) 원근 투영 시

    tRay                 m_Ray;
                         
    Matrix               m_matView;
    Matrix               m_matViewInv;
    Matrix               m_matProj;
    Matrix               m_matProjInv;

    // 물체 분류
    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_D;	// Deferred
    map<ULONG64, vector<tInstObj>>		m_mapInstGroup_F;	// Foward ( Opaque, Mask )	
    map<INT_PTR, vector<tInstObj>>		m_mapSingleObj;		// Single Object

    vector<CGameObject*>                m_vecDecal;         // Decal
    vector<CGameObject*>                m_vecTransparent;   // 투명, 반투명
    vector<CGameObject*>                m_vecEffect;
    vector<CGameObject*>                m_vecParticles;     // 투명, 반투명, 입자 타입
    vector<CGameObject*>                m_vecPostProcess;   // 후처리 오브젝트
    vector<CGameObject*>                m_vecUI;            // 후처리 오브젝트

    vector<CGameObject*> m_vecShadowMap;    // ShadowMap

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

    const tRay& GetRay()          { return m_Ray; }

    const Matrix& GetViewMat()    { return m_matView; }
    const Matrix& GetViewMatInv() { return m_matViewInv; }
    const Matrix& GetProjMat()    { return m_matProj; }
    const Matrix& GetProjMatInv() { return m_matProjInv; }

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

public:
    virtual void Begin() override;
    virtual void FinalTick() override;

public:
    void SortGameObject();
    void SortGameObject_ShadowMap();

    void render_deferred();
    void render_forward();
    void render_decal();

    void render_effect();
    void render_transparent();
    void render_particle();
    void render_postprocess();
    void render_ui();

    void render_shadowmap();
    void render_shadowblur();

    void clear();

private:
    void CalcRay();
        

public:
    CLONE(CCamera);
    CCamera();
    CCamera(const CCamera& _Other);
    virtual ~CCamera();

    friend class CRenderMgr;
};

