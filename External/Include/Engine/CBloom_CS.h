#pragma once
#include "CComputeShader.h"
class CBloom_CS :
    public CComputeShader
{
private:
    Ptr<CTexture> m_InputTex;
    Ptr<CTexture> m_OutputTex;

public:
    void SetInputTexture(Ptr<CTexture> _Tex) { m_InputTex = _Tex; }
    void SetOutputTexture(Ptr<CTexture> _Tex) { m_OutputTex = _Tex; }

public:
    virtual int  Binding()      override;
    virtual void CalcGroupNum() override;
    virtual void Clear()        override;

public:
    CBloom_CS();
    ~CBloom_CS();
};

