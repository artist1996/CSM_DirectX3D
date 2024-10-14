#pragma once
#include <Engine/CState.h>

class CDirezieAttack03State :
    public CState
{
private:
    Vec3 m_ArrivalPoint;
    Vec3 m_Dist;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CDirezieAttack03State);
    CDirezieAttack03State();
    virtual ~CDirezieAttack03State();
};

