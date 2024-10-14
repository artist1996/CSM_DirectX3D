#pragma once
#include <Engine/CState.h>

class CDirezieWakeUpState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CDirezieWakeUpState);
    CDirezieWakeUpState();
    virtual ~CDirezieWakeUpState();
};

