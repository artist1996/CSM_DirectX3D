#pragma once
#include <Engine/CState.h>

class CHyungteoBuffEaterState :
    public CState
{
private:
    Ptr<CPrefab> m_BuffEaterPref;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHyungteoBuffEaterState);
    CHyungteoBuffEaterState();
    virtual ~CHyungteoBuffEaterState();
};

