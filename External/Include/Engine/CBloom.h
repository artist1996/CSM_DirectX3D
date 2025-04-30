#pragma once
#include "CRenderComponent.h"
#include "CBloom_CS.h"

class CBloom :
    public CRenderComponent
{
private:
    Ptr<CTexture>   m_InputTex;
    Ptr<CTexture>   m_OutputTex;
    Ptr<CBloom_CS>  m_BlurCS;
    bool            m_Flag;

public:
    void SetRenderTarget(Ptr<CTexture> _Tex) { m_InputTex = _Tex; }

public:
    virtual void FinalTick() override;
    virtual void Render() override;

public:
    virtual void SaveToFile(FILE* _pFile) override;
    virtual void LoadFromFile(FILE* _pFile) override;

public:
    CLONE(CBloom);
    CBloom();
    ~CBloom();
};