#pragma once
#include "CComputeShader.h"

class CThresholdCS :
    public CComputeShader
{
private:
    Ptr<CTexture> m_InputTex;
    Ptr<CTexture> m_OutputTex;

public:
    void SetInputTex(Ptr<CTexture> _TargetTex)  { m_InputTex = _TargetTex; }
    void SetOutputTex(Ptr<CTexture> _TargetTex) { m_OutputTex = _TargetTex; }

public:
    virtual int  Binding()      override;
    virtual void CalcGroupNum() override;
    virtual void Clear()        override;

public:
    CThresholdCS();
    ~CThresholdCS();
};