#include "pch.h"
#include "LevelEditor.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>

#include "CEditorMgr.h"
#include "TreeUI.h"

#include "CollisionMatrix.h"

LevelEditor::LevelEditor()
	: m_CurLevel(nullptr)
	, m_strName{}
	, m_CheckCollsionEditor(false)
{
	SetActive(false);
}

LevelEditor::~LevelEditor()
{
}

void LevelEditor::Init()
{
}

void LevelEditor::Update()
{
	if (!IsActive() || !m_CurLevel)
		return;

	Ptr<CSound> pBGM = m_CurLevel->GetBGM();	


	ImGui::Text("Level Name");
	ImGui::SameLine(120);

	static char szBuff[255] = {};

	if (ImGui::InputText("##LevelName", szBuff, 255))
	{
		m_strName = szBuff;
		m_CurLevel->SetName(wstring(m_strName.begin(), m_strName.end()));
	}	

	ImGui::Text("BGM");
	ImGui::SameLine(120);

	string strBGM;
	if (nullptr != pBGM)
	{
		strBGM = string(pBGM->GetKey().begin(), pBGM->GetKey().end());
	}

	ImGui::InputText("##LevelBGM", (char*)strBGM.c_str(), strBGM.length(), ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::SOUND == pAsset->GetAssetType())
			{
				m_CurLevel->SetBGM((CSound*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}
	
	// BGM Play
	if (ImGui::Button("PLAY BGM", ImVec2(100.f, 25.f)))
	{
		pBGM->Play(0, 1.f, false);
	}
	ImGui::SameLine();

	if (ImGui::Button("STOP BGM", ImVec2(100.f, 25.f)))
	{
		pBGM->Stop();
	}

	ImGui::Text("Layer Collision Check");
	ImGui::SameLine();

	if (ImGui::Checkbox("##Level Collision Check", &m_CheckCollsionEditor))
	{
		CollisionMatrix* pEditor = (CollisionMatrix*)CEditorMgr::GetInst()->FindEditorUI("Layer Collision Matrix");
		pEditor->SetActive(m_CheckCollsionEditor);
	}
}

void LevelEditor::Activate()
{
	m_CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
}

void LevelEditor::Deactivate()
{
	m_CurLevel = nullptr;
	m_strName.clear();
}