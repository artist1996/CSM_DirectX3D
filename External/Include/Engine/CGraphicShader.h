#pragma once
#include "CShader.h"

struct tScalarParam
{
    SCALAR_PARAM ParamType;
    string       strDesc;
};

struct tTexParam
{
    TEX_PARAM    ParamType;
    string       strDesc;     
};

class CGraphicShader :
    public CShader
{
private:
    ComPtr<ID3D11VertexShader>    m_VS;
    ComPtr<ID3D11GeometryShader>  m_GS;
    ComPtr<ID3D11PixelShader>     m_PS;
    
    ComPtr<ID3DBlob>              m_VSBlob;
    ComPtr<ID3DBlob>              m_GSBlob;
    ComPtr<ID3DBlob>              m_PSBlob;

    ComPtr<ID3D11InputLayout>     m_Layout;

    D3D11_PRIMITIVE_TOPOLOGY      m_Topology;
    RS_TYPE                       m_RSType;
    DS_TYPE                       m_DSType;
    BS_TYPE                       m_BSType;

    SHADER_DOMAIN                 m_Domain;     // ½¦ÀÌ´õÀÇ ·»´õ¸µ ¹æ½Ä

    // Shader Parameter
    vector<tScalarParam>          m_vecScalarParam;
    vector<tTexParam>             m_vecTexParam;

public:
    void SetDomain(SHADER_DOMAIN _Domain)                { m_Domain = _Domain; }
    void SetRSType(RS_TYPE _Type)                        { m_RSType = _Type; }
    void SetDSType(DS_TYPE _Type)                        { m_DSType = _Type; }
    void SetBSType(BS_TYPE _Type)                        { m_BSType = _Type; }
    void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _Topology) { m_Topology = _Topology; }

    SHADER_DOMAIN GetDomain()              { return m_Domain; }
    RS_TYPE GetRSType()                    { return m_RSType; }
    DS_TYPE GetDSType()                    { return m_DSType; }
    BS_TYPE GetBSType()                    { return m_BSType; }
    D3D11_PRIMITIVE_TOPOLOGY GetTopology() { return m_Topology; }

    void AddScalarParam(SCALAR_PARAM _Type, const string& _Desc) { m_vecScalarParam.push_back(tScalarParam{ _Type, _Desc }); }
    void AddTexParam(TEX_PARAM _Type, const string& _Desc)       { m_vecTexParam.push_back(tTexParam{ _Type, _Desc }); }

    const vector<tScalarParam>& GetScalarParam() { return m_vecScalarParam; }
    const vector<tTexParam>& GetTexParam()       { return m_vecTexParam; }

public:
    int CreateVertexShader(const wstring& _RelativePath, const string& _FuncName);
    int CreateGeometryShader(const wstring& _RelativePath, const string& _FuncName);
    int CreatePixelShader(const wstring& _RelativePath, const string& _FuncName);
    

    virtual int Load(const wstring& _Key) override { return S_OK; }
    virtual int Save(const wstring& _Key) override { return S_OK; }

    void Binding();

public:
    CGraphicShader();
    virtual ~CGraphicShader();
};