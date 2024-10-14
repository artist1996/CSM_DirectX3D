#pragma once
#include <Engine/CState.h>

class CHyungteoDeadState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHyungteoDeadState);
    CHyungteoDeadState();
    virtual ~CHyungteoDeadState();
};

