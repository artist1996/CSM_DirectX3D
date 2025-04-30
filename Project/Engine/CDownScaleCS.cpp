#include "pch.h"
#include "CDownScaleCS.h"
#include "CDevice.h"

CDownScaleCS::CDownScaleCS()
	: CComputeShader(32, 32, 1, L"shader\\downscale.fx", "CS_DownScale")
{
}

CDownScaleCS::~CDownScaleCS()
{
}

int CDownScaleCS::Binding()
{
	if (nullptr == m_TargetTex || nullptr == m_DownScaleTex)
		return E_FAIL;

	if (nullptr == m_DownScaleTex->GetUAV() || nullptr == m_DownScaleTex->GetSRV())
		return E_FAIL;

	if (nullptr == m_TargetTex->GetSRV())
		return E_FAIL;


	m_TargetTex->Binding_CS_SRV(20);			// Threshold Texture
	m_DownScaleTex->Binding_CS_UAV(0);			// DownScale Texture

	m_Const.iArr[0] = m_TargetTex->Width();
	m_Const.iArr[1] = m_TargetTex->Height();
	m_Const.iArr[2] = m_DownScaleTex->Width();
	m_Const.iArr[3] = m_DownScaleTex->Height();

	return S_OK;
}

void CDownScaleCS::CalcGroupNum()
{
	if (nullptr == m_TargetTex || nullptr == m_DownScaleTex)
		return;

	m_GroupX = m_DownScaleTex->Width() / m_ThreadPerGroupX;
	m_GroupY = m_DownScaleTex->Height() / m_ThreadPerGroupY;
	m_GroupZ = 1;

	if (m_DownScaleTex->Width() % m_ThreadPerGroupX)
		m_GroupX += 1;
	if (m_DownScaleTex->Height() % m_ThreadPerGroupY)
		m_GroupY += 1;
}

void CDownScaleCS::Clear()
{
	m_DownScaleTex->Clear_CS_UAV();
}