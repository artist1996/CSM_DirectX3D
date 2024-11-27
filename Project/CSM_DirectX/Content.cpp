#include "pch.h"
#include "Content.h"
#include "ImGui/imgui.h"

#include "CEditorMgr.h"
#include "Inspector.h"

#include "TreeUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>
#include <Engine/CTaskMgr.h>
#include <Engine/CGameObject.h>

Content::Content()
	: m_Tree(nullptr)
{
	m_Tree = new TreeUI;
	m_Tree->SetName("ContentTree");
	AddChild(m_Tree);
	SetMove(false);
	m_Tree->SetShowRoot(false);
	m_Tree->UseDrag(true);
	m_Tree->SetShowNameOnly(true);
	m_Tree->AddClickedDelegate(this, (DELEGATE_1)&Content::AssetClicked);
	m_Tree->AddPopupDelegate(this, (DELEGATE_1)&Content::PopupMenu);

	RenewContent();
}

Content::~Content()
{
}

void Content::Init()
{
	Reload();
}

void Content::Update()
{
	if (CAssetMgr::GetInst()->IsChanged())
		RenewContent();
}

void Content::RenewContent()
{
	// 트리의 내용을 전부 제거
	m_Tree->Clear();

	// 부모노드를 지정하지 않음 == 루트노드 입력
	TreeNode* pRoot = m_Tree->AddNode(nullptr, "Root");
	
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		TreeNode* pNode = m_Tree->AddNode(pRoot, ToString((ASSET_TYPE)i));
		pNode->SetFrame(true);

		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);

		for (const auto& pair : mapAsset)
		{
			m_Tree->AddNode(pNode, string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second.Get());
		}
	}
}

void Content::AssetClicked(DWORD_PTR _Param)
{
	TreeNode* pNode = (TreeNode*)_Param;
	if (pNode->IsFrame())
		return;

	Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
	Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
	pInspector->SetTargetAsset(pAsset);
	//ImGui::SetWindowFocus(nullptr);
}

void Content::PopupMenu(DWORD_PTR _Param)
{
	TreeNode* pNode = (TreeNode*)_Param;
	
	Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();

	if (ASSET_TYPE::PREFAB == pAsset->GetAssetType())
	{
		if (ImGui::MenuItem("Instantiate"))
		{
			Ptr<CPrefab> pPrefab = (CPrefab*)pAsset.Get();

			CGameObject* CloneObj = pPrefab->Instantiate();

			if (L"Player" == CloneObj->GetName())
				CreateObject(CloneObj, 3);

			
			ImGui::CloseCurrentPopup();
		}
	}

	ImGui::EndPopup();
}

void Content::Reload()
{
	// Content 폴더에 있는 모든 Asset File들의 경로를 알아낸다.
	wstring ContentPath = CPathMgr::GetInst()->GetContentPath();
	FindAssetName(ContentPath, L"*.*");

	// 알아낸 Asset File 들의 경로를 통해 Asset 들을 AssetMgr에 Loading 한다.
	for (size_t i = 0; i < m_vecAssetPath.size(); ++i)
	{
		LoadAsset(m_vecAssetPath[i]);
	}

	// AssetMgr에는 존재 하지만 Content 폴더 내에 없는 Asset은 AssetMgr 에서 삭제 해준다.	
	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();
	
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		const map<wstring, Ptr<CAsset>>& mapAssets = CAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);
	
		for (const auto& pair : mapAssets)
		{
			if (pair.second->IsEngineAsset())
				continue;
	
			wstring strRelativePath = pair.second->GetRelativePath();
	
			// 절대경로로 Asset File이 존재 하는지 확인한다.
			if (false == std::filesystem::exists(strContentPath + strRelativePath))
			{
				// Asset 들은 스마트포인터로 관리하고 있기 때문에 RefCount 를 확인 후 1보다 크다면 다른 곳에서도 참조중이므로
				// 바로 삭제하지 않는다.
				if (pair.second->GetRefCount() <= 1)
				{
					CTaskMgr::GetInst()->AddTask(tTask{ TASK_TYPE::DELETE_ASSET, (DWORD_PTR)pair.second.Get() });
				}
				else
				{					
					int ret = MessageBox(nullptr, L"다른 곳에서 참조되고 있을 수 있습니다.\n 그래도 삭제 하시겠습니까?", L"Error", MB_YESNO);
					if (ret == IDYES)
					{
						CTaskMgr::GetInst()->AddTask(tTask{ TASK_TYPE::DELETE_ASSET, (DWORD_PTR)pair.second.Get(), });
					}
				}
			}
		}
	}
}

void Content::FindAssetName(const wstring& _FolderPath, const wstring& _Filter)
{
	WIN32_FIND_DATA tFindData = {};
	
	// 경로에 맞는 File 및 폴더를 탐색 할 수 있는 커널 오브젝트 생성
	wstring strFindPath = _FolderPath + _Filter;
	HANDLE hFinder = FindFirstFile(strFindPath.c_str(), &tFindData);

	assert(hFinder != INVALID_HANDLE_VALUE);

	// 탐색 커널 오브젝트를 이용 해 다음 파일을 반복해서 찾아낸다.
	while (FindNextFile(hFinder, &tFindData))
	{
		// 변수 이름은 FileName 이지만 FolderName 포함
		wstring strFindName = tFindData.cFileName;

		// 탐색한 File이 폴더 인지 식별해준다.
		if (tFindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			// Window 폴더 내엔 .. 이란 이름을 가진 보이지 않는 폴더가 존재한다. 상위 폴더로 접근 하는 폴더기 때문에 continue 해준다.
			if (strFindName == L"..")
				continue;
			// 다른 File 들을 다 찾아내야 하기 때문에 재귀함수를 통해 재귀적으로 모든 파일들을 찾아온다.
			FindAssetName(_FolderPath + strFindName + L"\\", _Filter);
		}
		// File 이라면 FolderPath 와 찾아낸 FileName 을 결합해 상대 경로를 받아 온 후 vector에 push_back 해준다
		else
		{
			wstring RelativePath = CPathMgr::GetInst()->GetRelativePath(_FolderPath + strFindName);
			m_vecAssetPath.push_back(RelativePath);
		}
	}

	// 커널 오브젝트 사용 후 메모리를 해제 해준다.
	FindClose(hFinder);
}

void Content::LoadAsset(const path& _Path)
{
	path ext = _Path.extension();

	if (ext == L".mesh")
		CAssetMgr::GetInst()->Load<CMesh>(_Path, _Path);
	else if (ext == L".mdat")
		CAssetMgr::GetInst()->Load<CMeshData>(_Path, _Path);
	else if (ext == L".mtrl")
		CAssetMgr::GetInst()->Load<CMaterial>(_Path, _Path);
	else if (ext == L".pref")
		CAssetMgr::GetInst()->Load<CPrefab>(_Path, _Path);
	else if (ext == L".png" || ext == L".jpg" || ext == L".jpeg" || ext == L".bmp" || ext == L".dds" || ext == L".tga"
		|| ext == L".PNG" || ext == L".JPG" || ext == L".JPEG" || ext == L".BMP" || ext == L".DDS" || ext == L".TGA")
		CAssetMgr::GetInst()->Load<CTexture>(_Path, _Path);
	else if (ext == L".mp3" || ext == L".mp4" || ext == L".ogg" || ext == L".wav" 
		|| ext == L".MP3" || ext == L".MP4" || ext == L".OGG" || ext == L".WAV")
		CAssetMgr::GetInst()->Load<CSound>(_Path, _Path);
	else if (ext == L".sprite")
		CAssetMgr::GetInst()->Load<CSprite>(_Path, _Path);
	else if (ext == L".anim")
		CAssetMgr::GetInst()->Load<CAnimation>(_Path, _Path);
}
