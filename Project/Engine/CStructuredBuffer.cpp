#include "pch.h"
#include "CStructuredBuffer.h"

#include "CDevice.h"

CStructuredBuffer::CStructuredBuffer()
	: m_Desc{}
	, m_SB_Write(nullptr)
	, m_ElementCount(0)
	, m_ElementSize(0)
	, m_RecentRegisterNum(0)
	, m_SysMemMove(false)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
}

int CStructuredBuffer::Create(UINT _ElementSize, UINT _ElementCount, SB_TYPE _Type, bool _SysMemMove, void* _InitData)
{
	assert(0 == (_ElementSize % 16));

	m_SB           = nullptr;
	m_SB_Write     = nullptr;
	m_SB_Read      = nullptr;
			       
	m_SRV          = nullptr;
	m_UAV          = nullptr;
	
	m_SysMemMove   = _SysMemMove;
	m_Type         = _Type;

	m_ElementSize  = _ElementSize;
	m_ElementCount = _ElementCount;

	// ID3D11Buffer 를 StructuredBuffer 용도로 생성하기
	if (SB_TYPE::SRV_UAV == _Type)
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;	
	else
		m_Desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

	m_Desc.ByteWidth		   = m_ElementSize * m_ElementCount;
	m_Desc.MiscFlags		   = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	m_Desc.StructureByteStride = m_ElementSize;

	if (!m_SysMemMove)
	{
		m_Desc.Usage		  = D3D11_USAGE_DYNAMIC;
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else
	{
		m_Desc.Usage		  = D3D11_USAGE_DEFAULT;
		m_Desc.CPUAccessFlags = 0;
	}

	HRESULT hr = S_OK;
	if (nullptr == _InitData)
	{
		hr = DEVICE->CreateBuffer(&m_Desc, nullptr, m_SB.GetAddressOf());
	}

	else
	{
		D3D11_SUBRESOURCE_DATA tSub = {};
		tSub.pSysMem = _InitData;
		hr = DEVICE->CreateBuffer(&m_Desc, &tSub, m_SB.GetAddressOf());
	}

	if (FAILED(hr))
		return E_FAIL;

	// 추가 Buffer 생성
	if (m_SysMemMove)
	{
		m_Desc.BindFlags	  = D3D11_BIND_SHADER_RESOURCE;
		m_Desc.Usage		  = D3D11_USAGE_DYNAMIC;
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		
		// Write Buffer 생성
		if (nullptr == _InitData)
		{		
			hr = DEVICE->CreateBuffer(&m_Desc, nullptr, m_SB_Write.GetAddressOf());
		}
		else
		{
			D3D11_SUBRESOURCE_DATA tSub = {};
			tSub.pSysMem = _InitData;
			hr = DEVICE->CreateBuffer(&m_Desc, &tSub, m_SB_Write.GetAddressOf());
		}

		// Read Buffer 생성
		m_Desc.Usage	      = D3D11_USAGE_DEFAULT;
		m_Desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		if (nullptr == _InitData)
		{
			hr = DEVICE->CreateBuffer(&m_Desc, nullptr, m_SB_Read.GetAddressOf());
		}
		else
		{
			D3D11_SUBRESOURCE_DATA tSub = {};
			tSub.pSysMem = _InitData;
			hr = DEVICE->CreateBuffer(&m_Desc, &tSub, m_SB_Read.GetAddressOf());
		}
	}


	// ShaderResourceView 생성하기
	D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};

	SRVDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFER;
	SRVDesc.BufferEx.NumElements = m_ElementCount;

	if (FAILED(DEVICE->CreateShaderResourceView(m_SB.Get(), &SRVDesc, m_SRV.GetAddressOf())))
	{
		return E_FAIL;
	}

	// Unordered Access View 생성
	
	if (SB_TYPE::SRV_UAV == m_Type)
	{
		D3D11_UNORDERED_ACCESS_VIEW_DESC tDesc = {};

		tDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		tDesc.Buffer.NumElements = m_ElementCount;
				
		if (FAILED(DEVICE->CreateUnorderedAccessView(m_SB.Get(), &tDesc, m_UAV.GetAddressOf())))
		{
			return E_FAIL;
		}
	}

	return S_OK;
}

void CStructuredBuffer::SetData(void* _pData, UINT _DataSize)
{
	// 입력데이터가 구조화버퍼 크기보다 작아야한다.
	assert(_DataSize <= m_Desc.ByteWidth);
	
	if (0 == _DataSize)
	{
		_DataSize = m_Desc.ByteWidth;
	}

	if (!m_SysMemMove)
	{
		D3D11_MAPPED_SUBRESOURCE tMapSub = {};
		CONTEXT->Map(m_SB.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tMapSub);
		memcpy(tMapSub.pData, _pData, _DataSize);
		CONTEXT->Unmap(m_SB.Get(), 0);
	}

	else
	{
		D3D11_MAPPED_SUBRESOURCE tMapSub = {};
		CONTEXT->Map(m_SB_Write.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tMapSub);
		memcpy(tMapSub.pData, _pData, _DataSize);
		CONTEXT->Unmap(m_SB_Write.Get(), 0);

		CONTEXT->CopyResource(m_SB.Get(), m_SB_Write.Get());
	}
}

void CStructuredBuffer::GetData(void* _pData, UINT _DataSize)
{
	assert(_DataSize <= m_Desc.ByteWidth);
	assert(m_SysMemMove);

	if (0 == _DataSize)
	{
		_DataSize = m_Desc.ByteWidth;
	}

	D3D11_MAPPED_SUBRESOURCE tMapSub = {};

	CONTEXT->CopyResource(m_SB_Read.Get(), m_SB.Get());

	CONTEXT->Map(m_SB_Read.Get(), 0, D3D11_MAP::D3D11_MAP_READ, 0, &tMapSub);
	memcpy(_pData, tMapSub.pData, _DataSize);
	CONTEXT->Unmap(m_SB_Read.Get(), 0);
}

void CStructuredBuffer::Binding(UINT _RegisterNum)
{
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CStructuredBuffer::Clear(UINT _RegisterNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->VSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_RegisterNum, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_RegisterNum, 1, &pSRV);
}

void CStructuredBuffer::Binding_CS_SRV(UINT _RegisterNum)
{
	m_RecentRegisterNum = _RegisterNum;
	CONTEXT->CSSetShaderResources(_RegisterNum, 1, m_SRV.GetAddressOf());
}

void CStructuredBuffer::Binding_CS_UAV(UINT _RegisterNum)
{
	m_RecentRegisterNum = _RegisterNum;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_RegisterNum, 1, m_UAV.GetAddressOf(), &i);
}

void CStructuredBuffer::Clear_CS_SRV()
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->CSSetShaderResources(m_RecentRegisterNum, 1, &pSRV);
}

void CStructuredBuffer::Clear_CS_UAV()
{
	ID3D11UnorderedAccessView* pUAV = nullptr;
	UINT i = 0;
	CONTEXT->CSSetUnorderedAccessViews(m_RecentRegisterNum, 1, &pUAV, &i);
}
