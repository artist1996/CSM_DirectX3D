#pragma once
#include <Engine/CScript.h>

class CHyungteoDownHitState :
    public CState
{
private:
    bool m_First;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHyungteoDownHitState);
    CHyungteoDownHitState();
    virtual ~CHyungteoDownHitState();
};

