#pragma once
#include <Engine/CState.h>

class CJurisStiffnessState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CJurisStiffnessState);
    CJurisStiffnessState();
    virtual ~CJurisStiffnessState();
};

