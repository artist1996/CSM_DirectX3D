#pragma once
#include <Engine/CState.h>

class CHyungteoStiffnessState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHyungteoStiffnessState);
    CHyungteoStiffnessState();
    virtual ~CHyungteoStiffnessState();
};

