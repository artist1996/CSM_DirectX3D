#pragma once
#include "CComponent.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "components.h"

enum class SCRIPT_PARAM
{
    INT,
    FLOAT,
    VEC2,
    VEC3,
    VEC4,
    TEXTURE,
    PREFAB,
};

struct tScriptParam
{
    SCRIPT_PARAM Type;
    string       strDesc;
    void*        pData;
    DWORD_PTR    Param0;
    DWORD_PTR    Param1;
};

class CScript :
    public CComponent
{
private:
    UINT                 m_ScriptType;
    vector<tScriptParam> m_vecScriptParam;

protected:
    void AddScriptParam(SCRIPT_PARAM _Type, const string& _strDesc, void* _pData, DWORD_PTR _Param0 = 0, DWORD_PTR _Param1 = 0)
    {
        m_vecScriptParam.push_back(tScriptParam{ _Type, _strDesc, _pData, _Param0, _Param1 });
    }

public:
    const vector<tScriptParam>& GetScriptParam() { return m_vecScriptParam; }
    
    void Instantiate(Ptr<CPrefab> _Pref, int _LayerIdx, Vec3 _WorldPos, const wstring& _strName = L"");

public:
    virtual void Begin() {}
    virtual void Tick() PURE;
    virtual void FinalTick() final override {}
    UINT GetScriptType() { return m_ScriptType; }

public:
    virtual void SaveToFile(FILE* _pFile)   PURE;
    virtual void LoadFromFile(FILE* _pFile) PURE;

public:
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObj, CCollider2D* _OtherCollider) {}
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)      {}
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObj, CCollider2D* _OtherCollider)   {}

public:
    CScript(UINT _m_ScriptType);
    virtual ~CScript();
};