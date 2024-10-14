#include "pch.h"
#include "CPrefab.h"
#include "CGameObject.h"

OBJECT_SAVE CPrefab::g_ObjectSaveFunc = nullptr;
OBJECT_LOAD CPrefab::g_ObjectLoadFunc = nullptr;

CPrefab::CPrefab()
	: CAsset(ASSET_TYPE::PREFAB)
	, m_ProtoObject(nullptr)
{
}

CPrefab::~CPrefab()
{
	SAFE_DELETE(m_ProtoObject);
}

int CPrefab::GetProtoObjectIdx()
{
	return m_ProtoObject->GetLayerIdx();
}

CGameObject* CPrefab::Instantiate()
{
	return m_ProtoObject->Clone();
}

int CPrefab::Save(const wstring& _FilePath)
{
	FILE* pFile = nullptr;

	_wfopen_s(&pFile, _FilePath.c_str(), L"wb");

	if (nullptr == pFile)
	{
		MessageBox(nullptr, L"Prefab Save Failed", L"Error", MB_OK);
		return E_FAIL;
	}

	g_ObjectSaveFunc(pFile, m_ProtoObject);

	fclose(pFile);

	return S_OK;
}

int CPrefab::Load(const wstring& _FilePath)
{
	FILE* pFile = nullptr;
	
	_wfopen_s(&pFile, _FilePath.c_str(), L"rb");
	
	if (nullptr == pFile)
	{
		MessageBox(nullptr, L"Prefab Load Failed", L"Error", MB_OK);
		return E_FAIL;
	}
	
	m_ProtoObject = g_ObjectLoadFunc(pFile);
	
	fclose(pFile);
	
	return S_OK;
}
