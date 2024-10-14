#pragma once
#include "CComponent.h"

enum class DATA_TYPE
{
    INT,
    FLOAT,
    VEC3,
    OBJECT,
};

struct tBlackboardData
{
    DATA_TYPE Type;
    void*     pData;
};

class CState;

class CFSM :
    public CComponent
{
private:
    map<wstring, tBlackboardData> m_mapData;
    map<wstring, CState*>         m_mapState;
    CState*                       m_CurState;

public:
    virtual void FinalTick() override;

public:
    void    AddState(const wstring& _strName, CState* _State);
    void    ChangeState(const wstring& _strName);
    CState* FindState(const wstring& _strName);

    void  SetBlackboardData(const wstring& _strKey, DATA_TYPE _Type, void* _pData);
    void* GetBlackboardData(const wstring& _strKey);

    CState* GetCurrentState()                           { return m_CurState; }

public:
    virtual void SaveToFile(FILE* _pFile)   override {}
    virtual void LoadFromFile(FILE* _pFile) override {}

public:
    CLONE(CFSM);
    CFSM();
    virtual ~CFSM();
};