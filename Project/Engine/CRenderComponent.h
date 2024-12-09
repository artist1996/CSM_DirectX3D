#pragma once
#include "CComponent.h"

struct tMtrlSet
{
    Ptr<CMaterial> pSharedMtrl;     // 공유 머테리얼
    Ptr<CMaterial> pDynamicMtrl;    // 공유 머테리얼 복사본
    Ptr<CMaterial> pCurMtrl;        // 현재 사용 할 머테리얼
};

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>          m_Mesh;
    vector<tMtrlSet>    m_vecMtrls;     // 재질

    bool                m_FrustumCheck; // 절두체 체크를 받을것인지 말것인지

public:
    void SetMesh(Ptr<CMesh> _Mesh);
    Ptr<CMesh> GetMesh() { return m_Mesh; }
    void SetMaterial(Ptr<CMaterial> _Mtrl, UINT _Idx);

    void SetFrustumCheck(bool _Check) { m_FrustumCheck = _Check; }
    bool IsFrustumCheck()             { return m_FrustumCheck; }

    Ptr<CMaterial> GetMaterial(UINT _Idx);
    Ptr<CMaterial> GetSharedMaterial(UINT _Idx);

    // 동적 재질 생성 및 반환
    Ptr<CMaterial> GetDynamicMaterial(UINT _Idx);

    UINT GetMaterialCount()           { return (UINT)m_vecMtrls.size(); }

    ULONG64 GetInstID(UINT _iMtrlIdx);

public:
    virtual void FinalTick() PURE;
    virtual void Render() PURE;
    virtual void Render(UINT _Subset);
    virtual CRenderComponent* Clone() PURE;

    virtual void render_shadowmap();
    virtual void render_shadowblur();

public:
    void SaveDataToFile(FILE* _pFile);
    void LoadDataFromFile(FILE* _pFile);

public:
    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& _Origin);
    virtual ~CRenderComponent();
};