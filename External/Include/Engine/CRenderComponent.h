#pragma once
#include "CComponent.h"

struct tMtrlSet
{
    Ptr<CMaterial> pSharedMtrl;     // ���� ���׸���
    Ptr<CMaterial> pDynamicMtrl;    // ���� ���׸��� ���纻
    Ptr<CMaterial> pCurMtrl;        // ���� ��� �� ���׸���
};

class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>          m_Mesh;
    vector<tMtrlSet>    m_vecMtrls;     // ����

    bool                m_FrustumCheck; // ����ü üũ�� ���������� ��������

public:
    void SetMesh(Ptr<CMesh> _Mesh);
    Ptr<CMesh> GetMesh() { return m_Mesh; }
    void SetMaterial(Ptr<CMaterial> _Mtrl, UINT _Idx);

    void SetFrustumCheck(bool _Check) { m_FrustumCheck = _Check; }
    bool IsFrustumCheck()             { return m_FrustumCheck; }

    Ptr<CMaterial> GetMaterial(UINT _Idx);
    Ptr<CMaterial> GetSharedMaterial(UINT _Idx);

    // ���� ���� ���� �� ��ȯ
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