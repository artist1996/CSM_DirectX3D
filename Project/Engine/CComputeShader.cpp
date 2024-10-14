#include "pch.h"
#include "CComputeShader.h"
#include "CDevice.h"
#include "CPathMgr.h"

#include "CConstBuffer.h"

CComputeShader::CComputeShader(UINT _ThreadPerGroupX, UINT _ThreadPerGroupY, UINT _ThreadPerGroupZ, const wstring& _strShaderRelativePath, const string& _strFuncName)
	: CShader(ASSET_TYPE::COMPUTE_SHADER)
	, m_ThreadPerGroupX(_ThreadPerGroupX)
	, m_ThreadPerGroupY(_ThreadPerGroupY)
	, m_ThreadPerGroupZ(_ThreadPerGroupZ)
	, m_GroupX(1)
	, m_GroupY(1)
	, m_GroupZ(1)
	, m_Const{}
{
	CreateComputeShader(_strShaderRelativePath, _strFuncName);
	SetEngineAsset();
}

CComputeShader::~CComputeShader()
{
}

int CComputeShader::Execute()
{
	// ComputeShader 를 상속 받은 class 들의 각자를 Binding 해준다
	if (FAILED(Binding()))
	{
		return E_FAIL;
	}

	// 상수 버퍼에 Data 를 세팅
	CConstBuffer* pConstBuffer = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pConstBuffer->Binding_CS();
	pConstBuffer->SetData(&m_Const);

	// 필요한 Group 수를 계산 해준다.
	CalcGroupNum();

	// Compute Shader 실행
	CONTEXT->CSSetShader(m_CS.Get(), 0, 0);
	CONTEXT->Dispatch(m_GroupX, m_GroupY, m_GroupZ);

	// Resource 해제
	Clear();

	return S_OK;
}

int CComputeShader::CreateComputeShader(const wstring& _strRelativePath, const string& _strFuncName)
{
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _strRelativePath;

	HRESULT hr = D3DCompileFromFile(strFilePath.c_str()
		, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), "cs_5_0", D3DCOMPILE_DEBUG, 0, m_CSBlob.GetAddressOf(), m_ErrBlob.GetAddressOf());

	if (FAILED(hr))
	{
		if (nullptr != m_ErrBlob)
		{
			MessageBoxA(nullptr, (char*)m_ErrBlob->GetBufferPointer(), "Compute Shader Compile Failed", MB_OK);
		}

		else
		{
			errno_t err = GetLastError();
			wchar_t szErrMsg[255] = {};

			swprintf_s(szErrMsg, 255, L"Error Code : %d", err);
			MessageBox(nullptr, szErrMsg, L"Compute Shader Compile Failed", MB_OK);
		}

		return E_FAIL;
	}

	DEVICE->CreateComputeShader(m_CSBlob->GetBufferPointer()
		, m_CSBlob->GetBufferSize(), nullptr, m_CS.GetAddressOf());

	return S_OK;
}

