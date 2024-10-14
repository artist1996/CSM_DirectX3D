#pragma once
#include <Engine/CState.h>

class CDirezieAttack01State :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CDirezieAttack01State);
    CDirezieAttack01State();
    virtual ~CDirezieAttack01State();
};

