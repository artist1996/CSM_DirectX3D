#include "pch.h"
#include "ScriptUI.h"

#include <Scripts/CScriptMgr.h>
#include <Engine/CScript.h>

#include "ParamUI.h"
#include "ListUI.h"

ScriptUI::ScriptUI()
	: ComponentUI(COMPONENT_TYPE::SCRIPT)
	, m_Script(nullptr)
	, m_SelectedPrefab(nullptr)
	, m_UIHeight(0)
{
}

ScriptUI::~ScriptUI()
{
}

void ScriptUI::Update()
{
	//Title();

	//const vector<CScript*>& vecScripts = GetTargetObject()->GetScripts();

	

	//if (ImGui::BeginCombo("Scripts", combo_preview_value, flags))
	//{
	//	for (int n = 0; n < IM_ARRAYSIZE(items); n++)
	//	{
	//		const bool is_selected = (item_current_idx == n);
	//		if (ImGui::Selectable(items[n], is_selected))
	//			item_current_idx = n;
	//
	//		// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
	//		if (is_selected)
	//			ImGui::SetItemDefaultFocus();
	//	}
	//	ImGui::EndCombo();
	//}
	m_UIHeight = 0;

	ImGui::PushID((int)GetComponentUIType());
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.7f, 0.3f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.7f, 0.3f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.7f, 0.3f, 0.5f));

	wstring strScriptName = CScriptMgr::GetScriptName(m_Script);
	ImGui::Button(string(strScriptName.begin(), strScriptName.end()).c_str());

	ImGui::PopStyleColor(3);
	ImGui::PopID();

	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	// Script 에 노출시킬 Data 를 보여준다.
	const vector<tScriptParam> vecParam = m_Script->GetScriptParam();

	for (size_t i = 0; i < vecParam.size(); ++i)
	{
		switch (vecParam[i].Type)
		{
		case SCRIPT_PARAM::INT:
			ParamUI::DragInt((int*)vecParam[i].pData, 1.f, vecParam[i].strDesc);
			break;
		case SCRIPT_PARAM::FLOAT:
			ParamUI::DragFloat((float*)vecParam[i].pData, 1.f, vecParam[i].strDesc);
			break;
		case SCRIPT_PARAM::VEC2:
			ParamUI::DragVec2((Vec2*)vecParam[i].pData, 1.f, vecParam[i].strDesc);
			break;
		case SCRIPT_PARAM::VEC3:
			ParamUI::DragVec3((Vec3*)vecParam[i].pData, 1.f, vecParam[i].strDesc);
			break;
		case SCRIPT_PARAM::VEC4:
			ParamUI::DragVec4((Vec4*)vecParam[i].pData, 1.f, vecParam[i].strDesc);
			break;
		case SCRIPT_PARAM::TEXTURE:
		{
			Ptr<CTexture>& pTex = *((Ptr<CTexture>*)vecParam[i].pData);
			ParamUI::InputTexture(pTex, vecParam[i].strDesc);
			m_UIHeight += 10;
		}
			break;
		case SCRIPT_PARAM::PREFAB:
		{
			Ptr<CPrefab>& pPrefab = *((Ptr<CPrefab>*)vecParam[i].pData);
			if (ParamUI::InputPrefab(pPrefab, vecParam[i].strDesc, this, (DELEGATE_1)&ScriptUI::SelectPrefab))
			{
				m_SelectedPrefab = &pPrefab;
			}
			m_UIHeight += 10;
		}
			break;
		}

		m_UIHeight += (int)ImGui::GetItemRectSize().y;
	}
	
	SetChildSize(ImVec2(0.f, (float)m_UIHeight + 10.f));
}

void ScriptUI::SetTargetScript(CScript* _Script)
{
	m_Script = _Script;

	if (nullptr != m_Script)
		SetActive(true);
	else
		SetActive(false);
}

void ScriptUI::SelectPrefab(DWORD_PTR _Param)
{
	ListUI* pList = (ListUI*)_Param;
	string strName = pList->GetSelectName();

	if ("None" == strName)
	{
		*m_SelectedPrefab = nullptr;
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());
	Ptr<CPrefab> pPrefab = CAssetMgr::GetInst()->FindAsset<CPrefab>(strAssetName);
	
	assert(pPrefab.Get());

	*m_SelectedPrefab = pPrefab;
}