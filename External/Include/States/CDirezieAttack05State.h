#pragma once
#include <Engine/CState.h>

class CDirezieAttack05State :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CDirezieAttack05State);
    CDirezieAttack05State();
    virtual ~CDirezieAttack05State();
};

