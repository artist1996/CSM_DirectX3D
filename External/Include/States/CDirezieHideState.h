#pragma once
#include <Engine/CState.h>

class CDirezieHideState :
    public CState
{
private:
    Ptr<CPrefab> m_CirclePref;
    CGameObject* m_Circle;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CDirezieHideState);
    CDirezieHideState();
    virtual ~CDirezieHideState();
};

