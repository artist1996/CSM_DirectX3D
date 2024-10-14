#pragma once
#include <Engine/CScript.h>

class CDirezieHitState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CDirezieHitState);
    CDirezieHitState();
    virtual ~CDirezieHitState();
};

