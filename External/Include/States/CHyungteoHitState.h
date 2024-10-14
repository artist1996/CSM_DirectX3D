#pragma once
#include <Engine/CState.h>

class CHyungteoHitState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHyungteoHitState);
    CHyungteoHitState();
    virtual ~CHyungteoHitState();
};

