#pragma once
#include <Engine/CState.h>

class CHyungteoHitBBQState :
    public CState
{
private:
    float m_InitHeight;
    float m_MaxHeight;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHyungteoHitBBQState);
    CHyungteoHitBBQState();
    virtual ~CHyungteoHitBBQState();
};

