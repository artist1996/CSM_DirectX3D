#include "pch.h"
#include "SE_Info.h"

#include "CEditorMgr.h"
#include "ListUI.h"
#include "TreeUI.h"

#include "SE_TextureView.h"
#include "SE_Create.h"

SE_Info::SE_Info()
	: m_Texture(nullptr)
	, m_Info{}
{
}

SE_Info::~SE_Info()
{
}

void SE_Info::Init()
{
}

void SE_Info::Update()
{
	Texture();

	TextureInfo();

	ImGui::Separator();

	SpriteInfo();

	ImGui::Separator();

	CreateSprite();
}

void SE_Info::Deactivate()
{
	m_Texture = nullptr;
	m_Info = {};
}

void SE_Info::Texture()
{
	string TexName;

	if (nullptr != m_Texture)
		TexName = string(m_Texture->GetKey().begin(), m_Texture->GetKey().end());

	ImGui::Text("Texture");
	ImGui::SameLine(100.f);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##AtlasTexture", (char*)TexName.c_str(), ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::TEXTURE == pAsset->GetAssetType())
			{
				SetTexture((CTexture*)pAsset.Get());
				m_Texture = (CTexture*)pAsset.Get();
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##AtlasTexBtn", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("Texture");
		vector<string> vecTexNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
		pListUI->AddList(vecTexNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&SE_Info::SelectTexture);
		pListUI->SetActive(true);
	}

	ImGui::SameLine();
	if (ImGui::Button("Load", ImVec2(37.f, 18.f)))
	{
		LoadTexture();
	}
}

void SE_Info::TextureInfo()
{
	UINT Width = 0;
	UINT Height = 0;
	
	if (nullptr != m_Texture)
	{
		Width = m_Texture->Width();
		Height = m_Texture->Height();
	}

	char szBuff[255] = {};
	sprintf_s(szBuff, 255, "%d", Width);
	
	ImGui::Text("Width");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##TextureWidth", szBuff, 255, ImGuiInputTextFlags_ReadOnly);

	sprintf_s(szBuff, 255, "%d", Height);
	ImGui::Text("Height");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##TextureHeight", szBuff, 255, ImGuiInputTextFlags_ReadOnly);
}

void SE_Info::SpriteInfo()
{
	ImGui::Text("LT");
	ImGui::SameLine(100);

	float fLT[] = { m_Info.LT.x, m_Info.LT.y };
	ImGui::InputFloat2("##SpriteInfoLeftTop", fLT, nullptr, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("SLICE");
	ImGui::SameLine(100);

	float fSlice[] = { m_Info.Slice.x, m_Info.Slice.y };
	ImGui::InputFloat2("##SpriteInfoSlice", fSlice, nullptr, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("OFFSET");
	ImGui::SameLine(100);
	ImGui::InputFloat2("##SpriteInfoOffset", (float*)&m_Info.Offset);

	ImGui::Text("BACKGROUND");
	ImGui::SameLine(100);
	ImGui::InputFloat2("##SpriteInfoBackground", (float*)&m_Info.BackGround);
}

void SE_Info::CreateSprite()
{
	if (nullptr == m_Texture)
		return;

	if (ImGui::Button("Create", ImVec2(50.f, 20.f)))
	{
		GetCreate()->SetTexture(m_Texture);
		GetCreate()->SetInfo(m_Info.LT, m_Info.Slice, m_Info.Offset, m_Info.BackGround);
		
		GetCreate()->Activate();
	}
}

void SE_Info::SelectTexture(DWORD_PTR _Param)
{
	ListUI* pListUI = (ListUI*)_Param;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		m_Texture = nullptr;
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);

	assert(pTex.Get());

	SetTexture(pTex);
}

void SE_Info::LoadTexture()
{
	wchar_t szSelect[256] = {};
	wchar_t szRelativePath[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szSelect;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szSelect);
	ofn.lpstrFilter = L"모든 파일\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;

	// 탐색창 초기 위치 지정
	wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"texture\\";
	ofn.lpstrInitialDir = strInitPath.c_str();

	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{
		wstring strConentPath = CPathMgr::GetInst()->GetContentPath();
		path RelativePath = std::filesystem::relative(szSelect, strConentPath);
		SetTexture(CAssetMgr::GetInst()->Load<CTexture>(RelativePath, RelativePath.wstring()));	
	}
}

void SE_Info::SetTexture(Ptr<CTexture> _Texture)
{
	m_Texture = _Texture;
	GetTextureView()->SetTexture(m_Texture);
}
