#pragma once
#include "CShader.h"
class CComputeShader :
    public CShader
{
private:
    ComPtr<ID3DBlob>            m_CSBlob;
    ComPtr<ID3D11ComputeShader> m_CS;

protected:
    // 1개의 그룹 당 쓰레드 개수
    const UINT                  m_ThreadPerGroupX;
    const UINT                  m_ThreadPerGroupY;
    const UINT                  m_ThreadPerGroupZ;

    // 그룹 개수
    UINT                        m_GroupX;
    UINT                        m_GroupY;
    UINT                        m_GroupZ;

    // 상수 데이터 전달 용도
    tMtrlConst                  m_Const;

public:
    int Execute();

private:
    int CreateComputeShader(const wstring& _strRelativePath, const string& _strFuncName);

    virtual int  Binding()      PURE;
    virtual void CalcGroupNum() PURE;
    virtual void Clear()        PURE;

public:
    CComputeShader(UINT _ThreadPerGroupX, UINT _ThreadPerGroupY, UINT _ThreadPerGroupZ, const wstring& _strShaderRelativePath, const string& _strFuncName);
    virtual ~CComputeShader();
};