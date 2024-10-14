#pragma once
#include <Engine/CState.h>

class CJurisBackAttackState :
    public CState
{
private:
    Ptr<CPrefab> m_AttackPref;
    bool         m_Init;
    bool         m_Spawn;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CJurisBackAttackState);
    CJurisBackAttackState();
    virtual ~CJurisBackAttackState();
};

