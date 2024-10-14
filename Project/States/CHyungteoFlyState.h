#pragma once
#include <Engine/CState.h>

class CHyungteoFlyState :
    public CState
{
private:
    float m_InitJumpPos;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHyungteoFlyState);
    CHyungteoFlyState();
    virtual ~CHyungteoFlyState();
};

