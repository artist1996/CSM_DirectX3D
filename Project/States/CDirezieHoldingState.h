#pragma once
#include <Engine/CState.h>

class CDirezieHoldingState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CDirezieHoldingState);
    CDirezieHoldingState();
    virtual ~CDirezieHoldingState();
};

