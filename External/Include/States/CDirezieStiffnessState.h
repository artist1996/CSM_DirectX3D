#pragma once
#include <Engine/CState.h>

class CDirezieStiffnessState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CDirezieStiffnessState);
    CDirezieStiffnessState();
    virtual ~CDirezieStiffnessState();
};

