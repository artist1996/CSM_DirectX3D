#pragma once
#include <Engine/CState.h>

class CJurisTraceState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CJurisTraceState);
    CJurisTraceState();
    virtual ~CJurisTraceState();
};

