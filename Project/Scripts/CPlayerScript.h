#pragma once
#include <Engine/CScript.h>

class CPlayerScript :
    public CScript
{
private:

private:
    void MouseCheck();

public:
    virtual void Begin() override;
    virtual void Tick() override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

public:
    CLONE(CPlayerScript);
    CPlayerScript();
    virtual ~CPlayerScript();
};

