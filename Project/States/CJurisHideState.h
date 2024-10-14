#pragma once
#include <Engine/CState.h>

class CJurisHideState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CJurisHideState);
    CJurisHideState();
    virtual ~CJurisHideState();
};

