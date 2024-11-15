#pragma once
#include "CEntity.h"
class CMRT :
    public CEntity
{
private:
    Ptr<CTexture>     m_arrRT[8];       // �ִ� ���� ������ RenderTarget ������ 8��
    Vec4              m_ClearColor[8];  
    UINT              m_RTCount;        // RenderTarget Count

    Ptr<CTexture>     m_DSTex;          // Depth Stencil Texture

    D3D11_VIEWPORT    m_ViewPort;      

public:
    Ptr<CTexture> GetRT(int _Index) { return m_arrRT[_Index]; }
    const D3D11_VIEWPORT& GetViewPort() { return m_ViewPort; }

public:
    void Create(int _RTCount, Ptr<CTexture>* _arrRT, Ptr<CTexture> _DSTex);
    void SetClearColor(Vec4* _Color, bool _Independent);

public:
    void OMSet();
    void Clear() { ClearRT(); ClearDS(); }
    void ClearRT();
    void ClearDS();

public:
    CLONE_DISABLE(CMRT);
    CMRT();
    virtual ~CMRT();
};

