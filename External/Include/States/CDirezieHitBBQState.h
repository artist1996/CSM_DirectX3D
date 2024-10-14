#pragma once
#include <Engine/CState.h>

class CDirezieHitBBQState :
    public CState
{
private:
    float m_MaxHeight;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CDirezieHitBBQState);
    CDirezieHitBBQState();
    virtual ~CDirezieHitBBQState();
};

