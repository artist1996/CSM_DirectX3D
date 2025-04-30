#include "pch.h"
#include "CVerticalBlurCS.h"

CVerticalBlurCS::CVerticalBlurCS()
	: CComputeShader(128,1,1,L"shader\\gaussianblur.fx", "CS_VerticalFilter")
{
}

CVerticalBlurCS::~CVerticalBlurCS()
{
}

int CVerticalBlurCS::Binding()
{
	if (nullptr == m_InputTex || nullptr == m_OutputTex)
		return E_FAIL;

	if (nullptr == m_OutputTex->GetUAV() || nullptr == m_OutputTex->GetSRV())
		return E_FAIL;

	if (nullptr == m_InputTex->GetSRV())
		return E_FAIL;


	m_InputTex->Binding_CS_SRV(20);			// DownScale    Texture
	m_OutputTex->Binding_CS_UAV(0);			// GaussianBlur Texture

	m_Const.iArr[0] = m_InputTex->Width();
	m_Const.iArr[1] = m_InputTex->Height();

	return S_OK;
}

void CVerticalBlurCS::CalcGroupNum()
{
	if (nullptr == m_OutputTex || nullptr == m_OutputTex)
		return;

	m_GroupX = (m_OutputTex->Width() + m_ThreadPerGroupX - 1) / m_ThreadPerGroupX;
	m_GroupY = m_OutputTex->Height();
	m_GroupZ = 1;
}

void CVerticalBlurCS::Clear()
{
	m_InputTex->Clear_CS_SRV();
	m_OutputTex->Clear_CS_UAV();
}