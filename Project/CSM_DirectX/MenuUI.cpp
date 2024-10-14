#include "pch.h"
#include "MenuUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>
#include <Engine/CScript.h>

#include <Scripts/CScriptMgr.h>

#include "Inspector.h"
#include "CEditorMgr.h"

#include "CLevelSaveLoad.h"
#include "CollisionMatrix.h"
#include "CreateGameObject.h"

MenuUI::MenuUI()
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::Tick()
{
	if (ImGui::BeginMainMenuBar())
	{
		Update();

		ImGui::EndMainMenuBar();
	}
}

void MenuUI::Update()
{
	File();

	Level();

	GameObject();

	Assets();

	Editor();
}

void MenuUI::File()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Level Save"))
		{
			SaveLevel();
		}

		if (ImGui::MenuItem("Level Load"))
		{
			LoadLevel();
		}

		ImGui::EndMenu();
	}
}

void MenuUI::Level()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	ImGui::BeginDisabled(!pCurLevel);

	if (ImGui::BeginMenu("Level"))
	{
		LEVEL_STATE State = LEVEL_STATE::STOP;

		if (nullptr != pCurLevel)
			State = pCurLevel->GetState();

		ImGui::BeginDisabled(LEVEL_STATE::PLAY == State);
		if (ImGui::MenuItem("Play"))
		{
			if (LEVEL_STATE::STOP == State)
			{
				wstring strLevelPath = CPathMgr::GetInst()->GetContentPath();
				strLevelPath += L"level\\Temp.lv";
				CLevelSaveLoad::SaveLevel(strLevelPath, pCurLevel);
			}

			ChangeLevelState(LEVEL_STATE::PLAY);
		}

		ImGui::EndDisabled();


		ImGui::BeginDisabled(LEVEL_STATE::PLAY != State);
		if (ImGui::MenuItem("Pause"))
		{
			ChangeLevelState(PAUSE);
		}
		ImGui::EndDisabled();

		ImGui::BeginDisabled(LEVEL_STATE::STOP == State);
		if (ImGui::MenuItem("Stop"))
		{
			wstring strLevelLoadPath = CPathMgr::GetInst()->GetContentPath();
			strLevelLoadPath += L"level\\Temp.lv";
			CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(strLevelLoadPath);
			ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);
			
			// Inspector Clear (이전 Object 정보를 보여줄 수 있기 때문에)
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			pInspector->SetTargetObject(nullptr);
			pInspector->SetTargetAsset(nullptr);
		}
		ImGui::EndDisabled();

		CollisionMatrix* pCollisionMatrix = (CollisionMatrix*)CEditorMgr::GetInst()->FindEditorUI("Layer Collision Matrix");
		bool IsActive = pCollisionMatrix->IsActive();

		if (ImGui::MenuItem("Layer Collision Matrix", nullptr, &IsActive))
		{
			pCollisionMatrix->SetActive(IsActive);
		}

		ImGui::EndMenu();
	}

	ImGui::EndDisabled();
}

void MenuUI::GameObject()
{
	if (ImGui::BeginMenu("GameObject"))
	{
		if (ImGui::MenuItem("Create Empty Object"))
		{
			CreateGameObject* pUI = (CreateGameObject*)CEditorMgr::GetInst()->FindEditorUI("Create GameObject");
			pUI->SetActive(true);
		}

		AddComponent();

		AddScript();

		ImGui::EndMenu();
	}
}

void MenuUI::Assets()
{
	if (ImGui::BeginMenu("Assets"))
	{
		if (ImGui::MenuItem("Create Empty Material"))
		{
			Ptr<CMaterial> pMtrl = new CMaterial;
			wstring Key = GetAssetKey(ASSET_TYPE::MATERIAL, L"Default Material");
			CAssetMgr::GetInst()->AddAsset(Key, pMtrl);
			pMtrl->Save(Key);
		}

		if (ImGui::MenuItem("Create Empty Prefab"))
		{
			Ptr<CPrefab> pPrefab = new CPrefab;
			wstring Key = GetAssetKey(ASSET_TYPE::PREFAB, L"Default Prefab");
			CAssetMgr::GetInst()->AddAsset(Key, pPrefab);
		}

		ImGui::EndMenu();
	}
}

void MenuUI::Editor()
{
	if (ImGui::BeginMenu("Editor"))
	{
		EditorUI* pSpriteEditor = CEditorMgr::GetInst()->FindEditorUI("Sprite Editor");
		bool IsActive = pSpriteEditor->IsActive();

		if (ImGui::MenuItem("Sprite Editor", nullptr, &IsActive))
		{
			CEditorMgr::GetInst()->FindEditorUI("Sprite Editor")->SetActive(IsActive);
		}

		EditorUI* pAnimationEditor = CEditorMgr::GetInst()->FindEditorUI("Animation Editor");
		bool IsAEActive = pAnimationEditor->IsActive();

		if (ImGui::MenuItem("Animation Editor", nullptr, &IsAEActive))
		{
			CEditorMgr::GetInst()->FindEditorUI("Animation Editor")->SetActive(IsAEActive);
		}

		EditorUI* pTileMapEditor = CEditorMgr::GetInst()->FindEditorUI("TileMap Editor");
		bool IsTEActive = pTileMapEditor->IsActive();

		if (ImGui::MenuItem("TileMap Editor", nullptr, &IsTEActive))
		{
			CEditorMgr::GetInst()->FindEditorUI("TileMap Editor")->SetActive(IsTEActive);
		}

		EditorUI* pLevelEditor = CEditorMgr::GetInst()->FindEditorUI("Level Editor");
		bool IsLVActive = pLevelEditor->IsActive();

		if (ImGui::MenuItem("Level Editor", nullptr, &IsLVActive))
		{
			pLevelEditor->SetActive(IsLVActive);
		}

		ImGui::EndMenu();
	}
}

void MenuUI::AddComponent()
{
	if (ImGui::BeginMenu("Add Component"))
	{
		if (ImGui::MenuItem("Transform"))
		{
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			CGameObject* pObject = pInspector->GetTargetObject();

			if (nullptr == pObject)
			{
				ImGui::EndMenu();
				return;
			}

			pObject->AddComponent(new CTransform);
		}

		if (ImGui::MenuItem("Collider2D"))
		{
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			CGameObject* pObject = pInspector->GetTargetObject();

			if (nullptr == pObject)
			{
				ImGui::EndMenu();
				return;
			}

			pObject->AddComponent(new CCollider2D);
		}

		if (ImGui::MenuItem("Animator2D"))
		{
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			CGameObject* pObject = pInspector->GetTargetObject();

			if (nullptr == pObject)
			{
				ImGui::EndMenu();
				return;
			}

			pObject->AddComponent(new CAnimator2D);
		}

		if (ImGui::MenuItem("Rigidbody"))
		{
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			CGameObject* pObject = pInspector->GetTargetObject();

			if (nullptr == pObject)
			{
				ImGui::EndMenu();
				return;
			}

			pObject->AddComponent(new CRigidbody);
		}

		if (ImGui::MenuItem("Camera"))
		{
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			CGameObject* pObject = pInspector->GetTargetObject();

			if (nullptr == pObject)
			{
				ImGui::EndMenu();
				return;
			}

			pObject->AddComponent(new CCamera);
		}

		if (ImGui::MenuItem("Light2D"))
		{
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			CGameObject* pObject = pInspector->GetTargetObject();

			if (nullptr == pObject)
			{
				ImGui::EndMenu();
				return;
			}

			pObject->AddComponent(new CLight2D);
		}

		if (ImGui::MenuItem("MeshRender"))
		{
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			CGameObject* pObject = pInspector->GetTargetObject();

			if (nullptr == pObject)
			{
				ImGui::EndMenu();
				return;
			}

			pObject->AddComponent(new CMeshRender);
		}

		if (ImGui::MenuItem("TileMap"))
		{
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			CGameObject* pObject = pInspector->GetTargetObject();

			if (nullptr == pObject)
			{
				ImGui::EndMenu();
				return;
			}
			if (pObject->GetRenderComponent())
				return;
			else
			{
				CTileMap* pTileMap = new CTileMap;
				
				pObject->AddComponent(pTileMap);
				pObject->TileMap()->SetTileSize(Vec2(64.f, 64.f));
				pObject->TileMap()->SetAtlasTileSize(Vec2(224.f, 120.f));
			}
		}

		if (ImGui::MenuItem("State Machine"))
		{
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			CGameObject* pObject = pInspector->GetTargetObject();

			if (nullptr == pObject)
			{
				ImGui::EndMenu();
				return;
			}

			pObject->AddComponent(new CFSM);
		}

		if (ImGui::MenuItem("Particle System"))
		{
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			CGameObject* pObject = pInspector->GetTargetObject();

			if (nullptr == pObject)
			{
				ImGui::EndMenu();
				return;
			}

			pObject->AddComponent(new CParticleSystem);
		}

		ImGui::EndMenu();
	}
}

void MenuUI::AddScript()
{
	//if (ImGui::BeginMenu("Add Script"))
	//{
	//	vector<wstring> vecScriptsName;
	//	CScriptMgr::GetScriptInfo(vecScriptsName);
	//
	//	for (size_t i = 0; i < vecScriptsName.size(); ++i)
	//	{
	//		if (ImGui::MenuItem(string(vecScriptsName[i].begin(), vecScriptsName[i].end()).c_str()))
	//		{
	//			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
	//
	//			CGameObject* pObject = pInspector->GetTargetObject();
	//
	//			if (nullptr != pObject)
	//			{
	//				CScript* pScript = CScriptMgr::GetScript(vecScriptsName[i]);
	//				pObject->AddComponent(pScript);
	//			}
	//		}	
	//	}
	//
	//	ImGui::EndMenu();
	//}
	if (ImGui::BeginMenu("Add Script"))
	{
		vector<wstring> vecScriptsName;
		CScriptMgr::GetScriptInfo(vecScriptsName);

		ImGui::BeginChild("ScrollableScripts", ImVec2(300, 200), true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

		for (size_t i = 0; i < vecScriptsName.size(); ++i)
		{
			if (ImGui::MenuItem(string(vecScriptsName[i].begin(), vecScriptsName[i].end()).c_str()))
			{
				Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");

				CGameObject* pObject = pInspector->GetTargetObject();

				if (nullptr != pObject)
				{
					CScript* pScript = CScriptMgr::GetScript(vecScriptsName[i]);
					pObject->AddComponent(pScript);
				}
			}
		}

		ImGui::EndChild();
		ImGui::EndMenu();
	}
}

void MenuUI::CreateAnimation()
{
	CEditorMgr::GetInst()->FindEditorUI("Input Animation Name")->SetActive(true);
}

wstring MenuUI::GetAssetKey(ASSET_TYPE _Type, const wstring& _strKey)
{
	wstring strKey;

	switch (_Type)
	{
	case ASSET_TYPE::MATERIAL:
	{
		strKey = wstring(L"material\\" + _strKey + L" %d.mtrl");
	}
		break;
	case ASSET_TYPE::PREFAB:
		strKey = wstring(L"prefab\\" + _strKey + L" %d.pref");
		break;
	case ASSET_TYPE::SPRITE:
		strKey = wstring(L"sprite\\" + _strKey + L" %d.sprite");
		break;
	case ASSET_TYPE::ANIMATION:
		strKey = wstring(L"flipbook\\" + _strKey + L" %d.flip");
		break;
	default:
		assert(nullptr);
		break;
	}

	wchar_t szKey[255] = {};
	wstring FilePath = CPathMgr::GetInst()->GetContentPath();

	for (UINT i = 0; i < 0xffffffff; ++i)
	{
		swprintf_s(szKey, 255, strKey.c_str(), i);
		
		if (false == std::filesystem::exists(FilePath + szKey))
		{
			break;
		}
	}

	return szKey;
}

void MenuUI::SaveLevel()
{
	wchar_t szSelect[256] = {};
	wchar_t szRelativePath[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szSelect;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szSelect);
	ofn.lpstrFilter = L"모든 파일\0*.*\0Level\0*.lv";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;

	// 탐색창 초기 위치 지정
	wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"level\\";
	ofn.lpstrInitialDir = strInitPath.c_str();

	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST |  OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	//OFN_OVERWRITEPROMPT |

	if (GetSaveFileName(&ofn))
	{	
		CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

		path path = szSelect;

		if (LEVEL_STATE::STOP != pCurLevel->GetState() || nullptr == pCurLevel)
			return;

		CLevelSaveLoad::SaveLevel(szSelect, pCurLevel);
	}
}

void MenuUI::LoadLevel()
{
	wchar_t szSelect[256] = {};
	wchar_t szRelativePath[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szSelect;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szSelect);
	ofn.lpstrFilter = L"모든 파일\0*.*\0Level\0*.lv";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;

	// 탐색창 초기 위치 지정
	wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"level\\";
	ofn.lpstrInitialDir = strInitPath.c_str();

	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{
		CLevel* pLevel = CLevelSaveLoad::LoadLevel(szSelect);
		ChangeLevel(pLevel, LEVEL_STATE::STOP);
	}

	Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
	pInspector->SetTargetObject(nullptr);
}