#include "pch.h"
#include "CHeightMapCS.h"

#include "CStructuredBuffer.h"

CHeightMapCS::CHeightMapCS()
    : CComputeShader(32, 32, 1, L"shader\\heightmap.fx", "CS_HeightMap")
{
}

CHeightMapCS::~CHeightMapCS()
{

}

int CHeightMapCS::Binding()
{
    if (nullptr == m_HeightMapTex)
        return E_FAIL;

    m_HeightMapTex->Binding_CS_UAV(0);
    m_RaycastOut->Binding_CS_SRV(20);

    m_Const.iArr[0] = (UINT)m_HeightMapTex->Width();
    m_Const.iArr[1] = (UINT)m_HeightMapTex->Height();

    m_Const.v2Arr[0] = m_BrushScale;

    if (nullptr != m_BrushTex)
    {
        m_BrushTex->Binding_CS_SRV(0);
        m_Const.btex[0] = 1;
    }
    else
        m_Const.btex[0] = 0;

    return S_OK;
}

void CHeightMapCS::CalcGroupNum()
{
    m_GroupX = (UINT)m_HeightMapTex->Width() / m_ThreadPerGroupX;
    m_GroupY = (UINT)m_HeightMapTex->Height() / m_ThreadPerGroupY;
    m_GroupZ = 1;

    if (0 < (UINT)m_HeightMapTex->Width() % m_ThreadPerGroupX)
        m_GroupX += 1;

    if (0 < (UINT)m_HeightMapTex->Height() % m_ThreadPerGroupY)
        m_GroupY += 1;
}

void CHeightMapCS::Clear()
{
    m_HeightMapTex->Clear_CS_UAV();
    m_HeightMapTex = nullptr;

    m_RaycastOut->Clear_CS_SRV();
    m_RaycastOut = nullptr;

    if (nullptr != m_BrushTex)
    {
        m_BrushTex->Clear_CS_SRV();
        m_BrushTex = nullptr;
    }
}
