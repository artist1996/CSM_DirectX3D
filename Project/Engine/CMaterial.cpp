#include "pch.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CAssetMgr.h"
#include "CConstBuffer.h"

CMaterial::CMaterial(bool _IsEngine)
	: CAsset(ASSET_TYPE::MATERIAL)
{
	if (_IsEngine)
	{
		SetEngineAsset();
	}
}

CMaterial::~CMaterial()
{
}

void* CMaterial::GetScalarParam(SCALAR_PARAM _Param)
{
	switch (_Param)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
		return m_Const.iArr + _Param;
		break;
	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		return m_Const.fArr + (_Param - FLOAT_0);
		break;
	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		return m_Const.v2Arr + (_Param - VEC2_0);
		break;
	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		return m_Const.v4Arr + (_Param - VEC4_0);
		break;
	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
		return m_Const.matArr + (_Param - MAT_0);	
		break;
	}
}

void CMaterial::Binding()
{
	if (!m_Shader)
		return;

	for (int i = 0; i < TEX_PARAM::END; ++i)
	{
		if (nullptr == m_arrTex[i])
		{
			m_Const.btex[i] = 0;
			CTexture::Clear(i);
			continue;
		}
			
		m_Const.btex[i] = 1;
		m_arrTex[i]->Binding(i);
	}
	
	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pCB->SetData(&m_Const);
	pCB->Binding();

	m_Shader->Binding();
}

int CMaterial::Save(const wstring& _RelativePath)
{
	SetRelativePath(_RelativePath);

	FILE* pFile = nullptr;

	wstring FilePath = CPathMgr::GetInst()->GetContentPath();

	FilePath += _RelativePath;

	_wfopen_s(&pFile, FilePath.c_str(), L"wb");

	if (nullptr == pFile)
		return E_FAIL;

	// 어떤 쉐이더를 참조 했는지
	SaveAssetRef(m_Shader, pFile);
	
	// 상수 데이터
	fwrite(&m_Const, sizeof(tMtrlConst), 1, pFile);
	
	for (UINT i = 0; i < TEX_PARAM::END; ++i)
	{
		SaveAssetRef(m_arrTex[i], pFile);
	}

	fclose(pFile);
	
	return S_OK;
}

int CMaterial::Load(const wstring& _FilePath)
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");

	if (nullptr == pFile)
		return E_FAIL;

	LoadAssetRef(m_Shader, pFile);

	fread(&m_Const, sizeof(tMtrlConst), 1, pFile);

	for (UINT i = 0; i < TEX_PARAM::END; ++i)
	{
		LoadAssetRef(m_arrTex[i], pFile);
	}

	fclose(pFile);

	return S_OK;
}
