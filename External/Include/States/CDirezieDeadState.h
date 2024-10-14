#pragma once
#include <Engine/CState.h>

class CDirezieDeadState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CDirezieDeadState);
    CDirezieDeadState();
    virtual ~CDirezieDeadState();
};

