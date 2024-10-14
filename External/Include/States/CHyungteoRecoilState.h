#pragma once
#include <Engine/CState.h>

class CHyungteoRecoilState :
    public CState
{
private:

public:
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE(CHyungteoRecoilState);
    CHyungteoRecoilState();
    virtual ~CHyungteoRecoilState();
};

