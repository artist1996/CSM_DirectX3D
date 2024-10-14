#pragma once
#include <Engine/CState.h>

class CHyungteoStingState :
    public CState
{
private:
    Ptr<CPrefab> m_StingPref;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHyungteoStingState);
    CHyungteoStingState();
    virtual ~CHyungteoStingState();
};

