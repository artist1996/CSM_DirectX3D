#pragma once
#include <Engine/CState.h>

class CJurisAttackState :
    public CState
{
private:
    Ptr<CPrefab> m_AttackPref;
    bool         m_Spawn;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CJurisAttackState);
    CJurisAttackState();
    virtual ~CJurisAttackState();
};

