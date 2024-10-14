#pragma once
#include <Engine/CState.h>

class CMeltKnightAttackState :
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
    CLONE(CMeltKnightAttackState);
    CMeltKnightAttackState();
    virtual ~CMeltKnightAttackState();
};

