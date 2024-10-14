#pragma once
#include <Engine/CState.h>

class CHyungteoEatState :
    public CState
{
private:
    float m_Time;
    bool  m_CamMove;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHyungteoEatState);
    CHyungteoEatState();
    virtual ~CHyungteoEatState();
};