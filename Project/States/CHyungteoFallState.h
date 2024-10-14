#pragma once
#include <Engine/CState.h>

class CHyungteoFallState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHyungteoFallState);
    CHyungteoFallState();
    virtual ~CHyungteoFallState();
};

