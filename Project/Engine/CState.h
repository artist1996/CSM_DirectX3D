#pragma once
#include "CAsset.h"

#include "CFSM.h"

class CState :
    public CEntity
{
private:
    CFSM* m_Owner;
    UINT  m_StateType;
    
protected:
    CGameObject* GetOwner() { return m_Owner->GetOwner(); }
    CFSM*        GetFSM()   { return m_Owner; }

public:
    UINT GetStateType()     { return m_StateType; }

public:
    virtual void Enter()     PURE;
    virtual void FinalTick() PURE;
    virtual void Exit()      PURE;

public:


public:
    CState(UINT _Type);
    virtual ~CState();

    friend class CFSM;
};

