#pragma once
#include <Engine/CState.h>

class CHyungteoIdleState :
    public CState
{
private:
    float m_Time;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHyungteoIdleState);
    CHyungteoIdleState();
    virtual ~CHyungteoIdleState();
};

