#include "pch.h"
#include "CUpScaleCS.h"

CUpScaleCS::CUpScaleCS()
	: CComputeShader(32,32,1,L"shader\\upscale.fx", "CS_UpScale")
{
}

CUpScaleCS::~CUpScaleCS()
{
}

int CUpScaleCS::Binding()
{
	if (nullptr == m_InputTex || nullptr == m_OutputTex)
		return E_FAIL;

	if (nullptr == m_OutputTex->GetUAV() || nullptr == m_OutputTex->GetSRV())
		return E_FAIL;

	if (nullptr == m_InputTex->GetSRV())
		return E_FAIL;


	m_InputTex->Binding_CS_SRV(20);			// Blur Texture
	m_OutputTex->Binding_CS_UAV(0);			// UpScale Texture

	m_Const.iArr[0] = m_OutputTex->Width();
	m_Const.iArr[1] = m_OutputTex->Height();
	m_Const.iArr[2] = m_InputTex->Width();
	m_Const.iArr[3] = m_InputTex->Height();

	return S_OK;
}

void CUpScaleCS::CalcGroupNum()
{
	if (nullptr == m_InputTex || nullptr == m_OutputTex)
		return;

	m_GroupX = m_OutputTex->Width() / m_ThreadPerGroupX;
	m_GroupY = m_OutputTex->Height() / m_ThreadPerGroupY;
	m_GroupZ = 1;

	if (m_OutputTex->Width() % m_ThreadPerGroupX)
		m_GroupX += 1;
	if (m_OutputTex->Height() % m_ThreadPerGroupY)
		m_GroupY += 1;
}

void CUpScaleCS::Clear()
{
	m_InputTex->Clear_CS_SRV();
	m_OutputTex->Clear_CS_UAV();
}