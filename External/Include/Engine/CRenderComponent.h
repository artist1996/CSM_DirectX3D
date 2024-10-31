#pragma once
#include "CComponent.h"

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>          m_Mesh;
    Ptr<CMaterial>      m_Mtrl;         // 현재 사용중인 재질
    Ptr<CMaterial>      m_SharedMtrl;   // 공유 재질(마스터)
    Ptr<CMaterial>      m_DynamicMtrl;  // 임시 재질

    bool                m_FrustumCheck;

public:
    void SetMesh(Ptr<CMesh> _Mesh)    { m_Mesh = _Mesh; }
    void SetMaterial(Ptr<CMaterial> _Mtrl);

    void SetFrustumCheck(bool _Check) { m_FrustumCheck = _Check; }
    bool IsFrustumCheck()             { return m_FrustumCheck; }

    Ptr<CMesh> GetMesh()              { return m_Mesh; }
    Ptr<CMaterial> GetMaterial()      { return m_Mtrl; }
    Ptr<CMaterial> GetSharedMaterial();
    Ptr<CMaterial> GetDynamicMaterial();

public:
    virtual void FinalTick() PURE;
    virtual void Render() PURE;
    virtual CRenderComponent* Clone() PURE;

public:
    void SaveDataToFile(FILE* _pFile);
    void LoadDataFromFile(FILE* _pFile);

public:
    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& _Origin);
    virtual ~CRenderComponent();
};