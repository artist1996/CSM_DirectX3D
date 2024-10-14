#include "pch.h"
#include "CreateGameObject.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

CreateGameObject::CreateGameObject()
	: m_LayerIdx(0)
{
	SetActive(false);
}

CreateGameObject::~CreateGameObject()
{
}

void CreateGameObject::Init()
{
}

void CreateGameObject::Update()
{
	if (!IsActive())
		return;

	ImGui::SetWindowFocus();

	ImGui::Text("Input Name");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200.f);

	char szBuff[255] = {};

	if (ImGui::InputText("##InputGameObjectName", szBuff, 255))
	{
		m_strName = szBuff;
	}

	ImGui::Text("Layer Index");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200.f);
	ImGui::InputInt("##InputGameObjectLayerIdx", &m_LayerIdx);

	if (ImGui::Button("OK", ImVec2(120, 0)))
	{
		wstring strName = wstring(m_strName.begin(), m_strName.end());

		CGameObject* pObject = new CGameObject;
		pObject->SetName(strName);
		CreateObject(pObject, m_LayerIdx);
		
		SetActive(false);
	}
	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();

	if (ImGui::Button("Cancel", ImVec2(120, 0)))
	{
		ImGui::CloseCurrentPopup();
		SetActive(false);
	}
}

void CreateGameObject::Deactivate()
{
	m_strName.clear();
	m_LayerIdx = 0;
}