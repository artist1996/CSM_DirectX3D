#pragma once
#include <Engine/CState.h>

class CMeltKnightDeadState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CMeltKnightDeadState);
    CMeltKnightDeadState();
    virtual ~CMeltKnightDeadState();
};

