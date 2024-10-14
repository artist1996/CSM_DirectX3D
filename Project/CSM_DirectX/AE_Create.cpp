#include "pch.h"
#include "AE_Create.h"

#include <Engine/CAssetMgr.h>
#include "AE_Detail.h"
#include "AE_Preview.h"

AE_Create::AE_Create()
{
	SetActive(false);
}

AE_Create::~AE_Create()
{
}

void AE_Create::Update()
{
	if (!IsActive())
		return;

	ImGui::SetWindowFocus();

	ImGui::Text("Input Name");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200.f);

	string strKey;
	char szBuff[255] = {};

	if (ImGui::InputText("##InputAnimationName", szBuff, 255))
	{
		m_strName = szBuff;
	}

	if (ImGui::Button("OK", ImVec2(120, 0)))
	{
		wstring Key = wstring(m_strName.begin(), m_strName.end());

		Ptr<CAnimation> pAnimation = new CAnimation;
			
		CAssetMgr::GetInst()->AddAsset(Key, pAnimation);
		GetDetail()->SetAnimation(pAnimation);
		GetPreview()->SetAnimation(pAnimation);
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

void AE_Create::Activate()
{
	m_strName.clear();
}

void AE_Create::Deactivate()
{
	m_strName.clear();
}
