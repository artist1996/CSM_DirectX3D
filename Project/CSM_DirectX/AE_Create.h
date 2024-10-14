#pragma once
#include "AE_Sub.h"
class AE_Create :
    public AE_Sub
{
private:
    string m_strName;

public:
    virtual void Update() override;

public:
    virtual void Activate() override;
    virtual void Deactivate() override;

public:
    AE_Create();
    virtual ~AE_Create();
};

