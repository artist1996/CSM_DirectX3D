#pragma once
#include <Engine/CState.h>

class CHyungteoLookState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHyungteoLookState);
    CHyungteoLookState();
    virtual ~CHyungteoLookState();
};

