#pragma once
#include <Engine/CState.h>

class CHyungteoAirHitState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHyungteoAirHitState);
    CHyungteoAirHitState();
    virtual ~CHyungteoAirHitState();
};

