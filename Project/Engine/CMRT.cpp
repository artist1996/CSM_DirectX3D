#include "pch.h"
#include "CMRT.h"

#include "CDevice.h"

CMRT::CMRT()
	: m_arrRT{}
	, m_ClearColor{}
	, m_ViewPort{}
	, m_RTCount(0)
{
}

CMRT::~CMRT()
{
}

void CMRT::Create(int _RTCount, Ptr<CTexture>* _arrRT, Ptr<CTexture> _DSTex)
{
	// MRT 개수
	m_RTCount = _RTCount;

	// MRT 세팅
	for (UINT i = 0; i < 8; ++i)
	{
		if (i < _RTCount)
			m_arrRT[i] = _arrRT[i];
		else
			m_arrRT[i] = nullptr;
	}

	// DepthStencil Tex 설정
	m_DSTex = _DSTex;

	Ptr<CTexture> pTarget = m_arrRT[0]; // 기본을 0번 Texture 로 지정
	
	if (nullptr == pTarget)
		pTarget = m_DSTex;

	assert(pTarget);

	// ViewPort 설정
	m_ViewPort.TopLeftX = 0.f;
	m_ViewPort.TopLeftY = 0.f;
	m_ViewPort.Width	= (float)pTarget->Width();
	m_ViewPort.Height	= (float)pTarget->Height();
	m_ViewPort.MinDepth = 0.f;
	m_ViewPort.MaxDepth = 1.f;
}

void CMRT::SetClearColor(Vec4* _Color, bool _Independent)
{
	if (_Independent)
	{
		for (UINT i = 0; i < m_RTCount; ++i)
		{
			m_ClearColor[i] = _Color[i];
		}
	}
	else
	{
		for (UINT i = 0; i < m_RTCount; ++i)
		{
			m_ClearColor[i] = _Color[0];
		}
	}
}

void CMRT::OMSet()
{
	ID3D11RenderTargetView* arrRTV[8] = {};
	
	for (UINT i = 0; i < m_RTCount; ++i)
	{
		arrRTV[i] = m_arrRT[i]->GetRTV().Get();
	}

	ComPtr<ID3D11DepthStencilView> pDSV = nullptr;

	if(nullptr != m_DSTex)
		pDSV = m_DSTex->GetDSV().Get();

	CONTEXT->OMSetRenderTargets((UINT)m_RTCount, arrRTV, pDSV.Get());
	CONTEXT->RSSetViewports(1, &m_ViewPort);
}

void CMRT::ClearRT()
{
	for (UINT i = 0; i < m_RTCount; ++i)
	{
		CONTEXT->ClearRenderTargetView(m_arrRT[i]->GetRTV().Get(), m_ClearColor[i]);
	}
}

void CMRT::ClearDS()
{
	if (nullptr == m_DSTex)
		return;
	
	CONTEXT->ClearDepthStencilView(m_DSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
}