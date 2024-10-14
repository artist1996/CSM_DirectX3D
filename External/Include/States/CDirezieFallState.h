#pragma once
#include <Engine/CState.h>

class CDirezieFallState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CDirezieFallState);
    CDirezieFallState();
    virtual ~CDirezieFallState();
};