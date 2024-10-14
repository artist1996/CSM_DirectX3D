#pragma once
#include <Engine/CState.h>

class CDirezieDownHitState :
    public CState
{
private:
    bool m_First;
    float m_Time;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CDirezieDownHitState);
    CDirezieDownHitState();
    virtual ~CDirezieDownHitState();
};

