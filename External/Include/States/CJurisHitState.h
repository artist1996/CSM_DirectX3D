#pragma once
#include <Engine/CState.h>

class CJurisHitState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CJurisHitState);
    CJurisHitState();
    virtual ~CJurisHitState();
};

