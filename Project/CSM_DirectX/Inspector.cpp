#include "pch.h"
#include "Inspector.h"
#include "ImGui/imgui.h"

#include "ComponentUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

#include <Engine/CGameObject.h>

#include "CEditorMgr.h"
#include "AssetUI.h"
#include "ScriptUI.h"
#include "ListUI.h"

Inspector::Inspector()
	: m_TargetObject(nullptr)
	, m_TargetAsset(nullptr)
	, m_arrComUI{}
	, m_arrAssetUI{}
{
	SetHorizontalScrollBar(false);
	SetMove(false);
}

Inspector::~Inspector()
{
}

void Inspector::Update()
{
	if (nullptr == m_TargetObject)
		return;

	SetTargetObject(m_TargetObject);

	// Target Object Name
	string strObjectName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
	ImGui::Text("Object Name");
	ImGui::SameLine(100);
	ImGui::InputText("##ObjectName", (char*)strObjectName.c_str(), strObjectName.length(), ImGuiInputTextFlags_ReadOnly);
	
	// Target Object Layer
	int LayerIdx = m_TargetObject->GetLayerIdx();
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pCurLevel->GetLayer(LayerIdx);
	string strLayerName = string(pLayer->GetName().begin(), pLayer->GetName().end());

	char szBuff[50] = {};
	if (strLayerName.empty())
	{
		sprintf_s(szBuff, 50, "%d : %s", LayerIdx, "None");
	}
	else
	{
		sprintf_s(szBuff, 50, "%d : %s", LayerIdx, strLayerName.c_str());
	}
	ImGui::Text("Layer");
	ImGui::SameLine(100);
	ImGui::InputText("##LayerName", szBuff, strlen(szBuff), ImGuiInputTextFlags_ReadOnly);

	int Idx = m_TargetObject->GetLayerIdx();

	ImGui::Text("Change Layer");
	ImGui::SameLine(100);

	if (ImGui::InputInt("##ChangeLayer", &Idx))
	{
		if (-1 == Idx)
			Idx = 0;
		if (Idx >= 31)
			Idx = 31;

		m_TargetObject->ChangeLayer(m_TargetObject, Idx);
	}

	if (ImGui::Button("Convert to Prefab", ImVec2(150.f, 32.f)))
	{
		Ptr<CPrefab> pPrefab = new CPrefab;
		
		pPrefab->SetProtoObject(m_TargetObject->Clone());
		
		wstring strFilePath = CPathMgr::GetInst()->GetContentPath();

		pPrefab->Save(strFilePath + L"prefab\\" + m_TargetObject->GetName() + L".pref");
	}

	ImGui::SameLine();

	if (ImGui::Button("DELETE OBJECT", ImVec2(150.f, 32.f)))
	{
		DeleteObject(m_TargetObject);
		SetTargetObject(nullptr);
	}
}

void Inspector::SetTargetObject(CGameObject* _Target)
{
	m_TargetObject = _Target;
	
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr == m_arrComUI[i])
			continue;

		m_arrComUI[i]->SetTargetObject(_Target);
	}

	if (nullptr == m_TargetObject)
	{
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->SetTargetScript(nullptr);
		}
	}
	else
	{
		const vector<CScript*>& vecScripts = m_TargetObject->GetScripts();

		if (m_vecScriptUI.size() < vecScripts.size())
		{
			CreateScriptUI(UINT(vecScripts.size() - m_vecScriptUI.size()));
		}

		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			if (i < vecScripts.size())
				m_vecScriptUI[i]->SetTargetScript(vecScripts[i]);
			else
				m_vecScriptUI[i]->SetTargetScript(nullptr);
		}
	}

	// AssetUI 비활성화
	m_TargetAsset = nullptr;

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->SetAsset(nullptr);
	}
}

void Inspector::SetTargetAsset(Ptr<CAsset> _Asset)
{
	if (nullptr == _Asset)
		return;

	SetTargetObject(nullptr);

	m_TargetAsset = _Asset;
	
	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->SetAsset(m_TargetAsset);
	}
}