#pragma once
#include <Engine/CState.h>

class CMeltKnightTraceState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CMeltKnightTraceState);
    CMeltKnightTraceState();
    virtual ~CMeltKnightTraceState();
};

