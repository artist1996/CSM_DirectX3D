#pragma once
#include "CComputeShader.h"
class CShadowBlurCS :
    public CComputeShader
{
private:
    Ptr<CTexture> m_InputTex;
    Ptr<CTexture> m_OutputTex;

public:
    void SetInputTex(Ptr<CTexture> _Tex) { m_InputTex = _Tex; }
    void SetOutputTex(Ptr<CTexture> _Tex) { m_OutputTex = _Tex; }

public:
    virtual int  Binding()      override;
    virtual void CalcGroupNum() override;
    virtual void Clear()        override;

public:
    CShadowBlurCS();
    ~CShadowBlurCS();
};

