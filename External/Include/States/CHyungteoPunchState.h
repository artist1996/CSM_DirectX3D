#pragma once
#include <Engine/CState.h>

class CHyungteoPunchState :
    public CState
{
private:
    Ptr<CPrefab> m_PunchPref;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHyungteoPunchState);
    CHyungteoPunchState();
    virtual ~CHyungteoPunchState();
};

