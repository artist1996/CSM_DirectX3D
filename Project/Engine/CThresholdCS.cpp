#include "pch.h"
#include "CThresholdCS.h"

CThresholdCS::CThresholdCS()
    : CComputeShader(32,32,1,L"shader\\threshold.fx","CS_Threshold")
{
}

CThresholdCS::~CThresholdCS()
{
}

int CThresholdCS::Binding()
{
	if (nullptr == m_InputTex || nullptr == m_OutputTex)
		return E_FAIL;

	if (nullptr == m_OutputTex->GetUAV() || nullptr == m_OutputTex->GetSRV())
		return E_FAIL;

	if (nullptr == m_InputTex->GetSRV())
		return E_FAIL;


	m_InputTex->Binding_CS_SRV(20);			// Threshold Texture
	m_OutputTex->Binding_CS_UAV(0);			// DownScale Texture

	m_Const.iArr[0] = m_InputTex->Width();
	m_Const.iArr[1] = m_InputTex->Height();

	return S_OK;
}

void CThresholdCS::CalcGroupNum()
{
	if (nullptr == m_OutputTex || nullptr == m_OutputTex)
		return;

	m_GroupX = m_OutputTex->Width() / m_ThreadPerGroupX;
	m_GroupY = m_OutputTex->Height() / m_ThreadPerGroupY;
	m_GroupZ = 1;

	if (m_OutputTex->Width() % m_ThreadPerGroupX)
		m_GroupX += 1;
	if (m_OutputTex->Height() % m_ThreadPerGroupY)
		m_GroupY += 1;
}

void CThresholdCS::Clear()
{
	m_InputTex->Clear_CS_SRV();
	m_OutputTex->Clear_CS_UAV();
}