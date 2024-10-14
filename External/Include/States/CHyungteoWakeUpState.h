#pragma once
#include <Engine/CState.h>
class CHyungteoWakeUpState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHyungteoWakeUpState);
    CHyungteoWakeUpState();
    virtual ~CHyungteoWakeUpState();
};

