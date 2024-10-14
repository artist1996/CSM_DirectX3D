#pragma once
#include <Engine/CState.h>

class CJurisDeadState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CJurisDeadState);
    CJurisDeadState();
    virtual ~CJurisDeadState();
};

