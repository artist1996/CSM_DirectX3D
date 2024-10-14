#include "pch.h"
#include "PrefabUI.h"

#include <Engine/CPathMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CGameObject.h>

#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include "CEditorMgr.h"
#include "EditorUI.h"
#include "Inspector.h"
#include "Content.h"

#include "TreeUI.h"

PrefabUI::PrefabUI()
	: AssetUI(ASSET_TYPE::PREFAB)
	, m_Idx(0)
{
}

PrefabUI::~PrefabUI()
{
}

void PrefabUI::Update()
{
	Title();

	Ptr<CPrefab> pPrefab = (CPrefab*)GetAsset().Get();
	CGameObject* pProtoObject = pPrefab->GetProtoObject();

	string strName;
	if (nullptr == pProtoObject)
		strName = "";
	else
		strName = string(pProtoObject->GetName().begin(), pProtoObject->GetName().end());

	ImGui::Text("Name");
	ImGui::SameLine(85.f);
	ImGui::InputText("##ProtoObjectName", (char*)strName.c_str(), 255, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Layer Info");
	ImGui::SameLine(85.f);
	CLayer* pLayer = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(m_Idx);
	
	if (pLayer->GetName().empty())
		strName = "None";
	else
		strName = string(pLayer->GetName().begin(), pLayer->GetName().end());
	
	ImGui::InputText("##LayerIndexName", (char*)strName.c_str(), strName.length(), ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Layer Index");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.f);
	if (ImGui::InputInt("##PrefabSpawnIdx", &m_Idx))
	{
		if (0 >= m_Idx)
			m_Idx = 0;
		else if (31 <= m_Idx)
			m_Idx = 31;
	}

	ImGui::Separator();

	if (ImGui::Button("Instantiate", ImVec2(100.f, 18.f)))
	{
		CGameObject* pCloneObj = pPrefab->Instantiate();
		CreateObject(pCloneObj, m_Idx);
	}
}