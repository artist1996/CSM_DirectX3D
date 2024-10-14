#pragma once
#include <Engine/CState.h>

class CDirezieAttack04State :
    public CState
{
private:
    float m_Time;
    Ptr<CPrefab> m_Pref;
    CGameObject* m_Clone;

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CDirezieAttack04State);
    CDirezieAttack04State();
    virtual ~CDirezieAttack04State();
};

