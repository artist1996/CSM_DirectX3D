#pragma once
#include <Engine/CState.h>

class CMeltKnightStandByState :
    public CState
{
private:
    float m_Time;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CMeltKnightStandByState);
    CMeltKnightStandByState();
    virtual ~CMeltKnightStandByState();
};

