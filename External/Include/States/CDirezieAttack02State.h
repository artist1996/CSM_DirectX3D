#pragma once
#include <Engine/CState.h>

class CDirezieAttack02State :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CDirezieAttack02State);
    CDirezieAttack02State();
    virtual ~CDirezieAttack02State();
};

