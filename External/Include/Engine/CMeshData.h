#pragma once
#include "CAsset.h"

#include "CMesh.h"
#include "CMaterial.h"

class CMeshData :
    public CAsset
{
private:
    Ptr<CMesh>             m_pMesh;
    vector<Ptr<CMaterial>> m_vecMtrl;

public:
    static CMeshData* LoadFromFBX(const wstring& _RelativePath);
    
public:
    CGameObject* Instantiate();

public:
    virtual int Save(const wstring& _strFilePath) override;
    virtual int Load(const wstring& _strFilePath) override;

public:
    CLONE_DISABLE(CMeshData);
    CMeshData(bool _Engine = false);
    ~CMeshData();
};