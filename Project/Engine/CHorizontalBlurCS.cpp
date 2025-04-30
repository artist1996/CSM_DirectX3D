#include "pch.h"
#include "CHorizontalBlurCS.h"

CHorizontalBlurCS::CHorizontalBlurCS()
	: CComputeShader(1, 128, 1, L"shader\\horizontal.fx", "CS_HorizFilter")
	, m_InputTex(nullptr)
{
}

CHorizontalBlurCS::~CHorizontalBlurCS()
{
}


int CHorizontalBlurCS::Binding()
{
	if (nullptr == m_InputTex || nullptr == m_OutputTex)
		return E_FAIL;

	if (nullptr == m_OutputTex->GetUAV() || nullptr == m_OutputTex->GetSRV())
		return E_FAIL;

	if (nullptr == m_InputTex->GetSRV())
		return E_FAIL;


	m_InputTex->Binding_CS_SRV(20);			// Vertical   Texture
	m_OutputTex->Binding_CS_UAV(0);			// Horizontal Texture

	m_Const.iArr[0] = m_InputTex->Width();
	m_Const.iArr[1] = m_InputTex->Height();

	return S_OK;
}

void CHorizontalBlurCS::CalcGroupNum()
{
	if (nullptr == m_OutputTex || nullptr == m_OutputTex)
		return;

	m_GroupX = m_OutputTex->Width();
	m_GroupY = UINT(m_OutputTex->Height() + m_ThreadPerGroupY - 1) / m_ThreadPerGroupY;
	m_GroupZ = 1;
}

void CHorizontalBlurCS::Clear()
{
	m_InputTex->Clear_CS_SRV();
	m_OutputTex->Clear_CS_UAV();
}