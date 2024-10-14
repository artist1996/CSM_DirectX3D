#pragma once
#include <Engine/CState.h>

class CDirezieAirHitState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CDirezieAirHitState);
    CDirezieAirHitState();
    virtual ~CDirezieAirHitState();
};

