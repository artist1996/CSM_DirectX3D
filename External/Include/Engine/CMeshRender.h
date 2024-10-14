#pragma once
#include "CRenderComponent.h"

class CMeshRender :
    public CRenderComponent
{
private:

public:
    virtual void FinalTick() override;
    virtual void Render() override;
    
public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

public:
    virtual CMeshRender* Clone() { return new CMeshRender(*this); }
    CMeshRender();
    virtual ~CMeshRender();
};