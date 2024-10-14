#pragma once

#include "CPathMgr.h"
#include "CTaskMgr.h"

class CAsset;
class CTexture;

class CAssetMgr
	: public CSingleton<CAssetMgr>
{
	SINGLE(CAssetMgr);
private:
	map<wstring, Ptr<CAsset>> m_mapAsset[(UINT)ASSET_TYPE::END];
	bool				      m_Changed;

public:
	void Tick();
	void Init();

public:
	template<typename T>
	Ptr<T> Load(const wstring& _Key, const wstring& _RelativePath);

	Ptr<CAsset> FindAsset(ASSET_TYPE _Type, const wstring& _Key);

	template <typename T>
	void AddAsset(const wstring& _Key, Ptr<T> _Asset);

	template <typename T>
	Ptr<T> FindAsset(const wstring& _Key);

	// _Flags : D3D11_BIND_FLAG
	Ptr<CTexture> CreateTexture(const wstring& _strKey, UINT _Width, UINT _Height
							  , DXGI_FORMAT _Format, UINT _Flags
							  , D3D11_USAGE _Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT);

	Ptr<CTexture> CreateTexutre(const wstring& _Key, ComPtr<ID3D11Texture2D> _Tex2D);

	void GetAssetNames(ASSET_TYPE _Type, vector<string>& _vecOut);
	const map<wstring, Ptr<CAsset>>& GetAssets(ASSET_TYPE _Type) { return m_mapAsset[(UINT)_Type]; }
	bool IsChanged() { return m_Changed; }

private:
	void DeleteAsset(ASSET_TYPE _Type, const wstring& _Key);

private:
	void CreateEngineMesh();
	void CreateEngineMaterial();
	void CreateEngineTexture();
	void CreateEngineSprite();
	void CreateEngineGraphicShader();
	void CreateEngineComputeShader();

	friend class CTaskMgr;
};

template<typename T>
Ptr<T> CAssetMgr::Load(const wstring& _Key, const wstring& _RelativePath)
{
	// 동일 키값 에셋 확인
	Ptr<T> Asset = FindAsset<T>(_Key);

	if (nullptr != Asset)
	{
		return Asset;
	}
	
	// 동일 키 값의 에셋이 없었으면
	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _RelativePath;

	Asset = new T;

	// 로딩 실패 시 예외 처리
	if (FAILED(Asset->Load(strFilePath)))
	{
		MessageBox(nullptr, L"알 수 없는 텍스쳐 포맷", L"텍스쳐 로딩 실패", MB_OK);
		return nullptr;
	}
	
	// Asset 이 자신의 키값과 경로를 알게 한다.
	Asset->m_Key = _Key;
	Asset->m_RelativePath = _RelativePath;

	// 맵에 등록
	ASSET_TYPE Type = GetAssetType<T>();
	m_mapAsset[(UINT)Type].insert(make_pair(_Key, Asset.Get()));

	CTaskMgr::GetInst()->AddTask(tTask{ ASSET_CHANGED });

	// 로딩된 에셋 주소 반환
	return Asset;
}


template<typename T>
Ptr<T> CAssetMgr::FindAsset(const wstring& _Key)
{
	ASSET_TYPE Type = GetAssetType<T>();
	
	map<wstring, Ptr<CAsset>>::iterator iter = m_mapAsset[(UINT)Type].find(_Key);
	
	if (iter == m_mapAsset[(UINT)Type].end())
		return nullptr;

	return (T*)iter->second.Get();
}

template<typename T>
void CAssetMgr::AddAsset(const wstring& _Key, Ptr<T> _Asset)
{
	ASSET_TYPE Type = GetAssetType<T>();
	
	assert(!FindAsset(Type, _Key).Get());
	
	_Asset->SetKey(_Key);
	m_mapAsset[(UINT)Type].insert(make_pair(_Key, _Asset.Get()));

	CTaskMgr::GetInst()->AddTask(tTask{ ASSET_CHANGED });
}

template<typename T>
void SaveAssetRef(Ptr<T> _Asset, FILE* _pFile)
{
	// nullptr 면 false 아니라면 true
	bool bAsset = _Asset.Get();
	fwrite(&bAsset, 1, 1, _pFile);

	// true 라면 Key 값과 RelativePath 값 저장
	if (bAsset)
	{
		SaveWString(_Asset->GetKey(), _pFile);
		SaveWString(_Asset->GetRelativePath(), _pFile);
	}
}

template<typename T>
void LoadAssetRef(Ptr<T>& _Asset, FILE* _pFile)
{
	bool bAsset = false;
	
	// 저장 해놓은 bool 값 읽어 온 후 true 일 때 Key 값과 RelativePath 읽어와서 Texture Load
	fread(&bAsset, 1, 1, _pFile);

	if (bAsset)
	{
		wstring Key, RelativePath;
		LoadWString(Key, _pFile);
		LoadWString(RelativePath, _pFile);

		_Asset = CAssetMgr::GetInst()->Load<T>(Key, RelativePath);
	}
}