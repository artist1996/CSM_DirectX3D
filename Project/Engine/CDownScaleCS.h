#pragma once
#include "CComputeShader.h"

class CDownScaleCS :
    public CComputeShader
{
private:
    Ptr<CTexture> m_TargetTex;
    Ptr<CTexture> m_DownScaleTex;

public:
    void SetTargetTex(Ptr<CTexture> _TargetTex)     { m_TargetTex = _TargetTex; }
    void SetDownScaleTex(Ptr<CTexture> _TargetTex)  { m_DownScaleTex = _TargetTex; }

public:
    virtual int  Binding()      override;
    virtual void CalcGroupNum() override;
    virtual void Clear()        override;

public:
     CDownScaleCS();
    ~CDownScaleCS();
};

