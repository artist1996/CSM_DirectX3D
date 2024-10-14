#pragma once
#include "CAsset.h"

#include "CEngine.h"

class CPrefab :
    public CAsset
{
private:
    static OBJECT_SAVE g_ObjectSaveFunc;
    static OBJECT_LOAD g_ObjectLoadFunc;

private:
    class CGameObject* m_ProtoObject;

public:
    void SetProtoObject(CGameObject* _Object) { m_ProtoObject = _Object; }
    int  GetProtoObjectIdx();
    CGameObject* Instantiate();

    CGameObject* GetProtoObject() { return m_ProtoObject; }

public:
    virtual int Save(const wstring& _FilePath) override;
    virtual int Load(const wstring& _FilePath) override;

public:
    CPrefab();
    virtual ~CPrefab();

    friend class CEngine;
};

