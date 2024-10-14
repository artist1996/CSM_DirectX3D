#pragma once
#include <Engine/CState.h>

class CHyungteoTraceState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHyungteoTraceState);
    CHyungteoTraceState();
    virtual ~CHyungteoTraceState();
};

