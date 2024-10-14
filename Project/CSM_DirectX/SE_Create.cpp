#include "pch.h"
#include "SE_Create.h"

#include "SE_Info.h"

SE_Create::SE_Create()
	: m_Texture(nullptr)
{
	SetActive(false);
}

SE_Create::~SE_Create()
{
}

void SE_Create::Init()
{
}

void SE_Create::Update()
{
	if (nullptr == m_Texture || !IsActive())
		return;
		
	//ImGui::SetWindowFocus();

	ImGui::Text("Sprite Name");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(200.f);

	string strKey;
	char szBuff[255] = {};

	if (ImGui::InputText("##InputName", szBuff, 255))
	{
		m_strName = szBuff;
	}

	if (ImGui::Button("OK", ImVec2(120, 0)))
	{
		wstring Key = wstring(m_strName.begin(), m_strName.end());

		Ptr<CSprite> pSprite = new CSprite;
		
		pSprite->Create(m_Texture, m_LT, m_Slice);
		pSprite->SetOffsetUV(m_Offset);
		pSprite->SetBackgroundUV(m_BackGround);
		CAssetMgr::GetInst()->AddAsset(Key, pSprite);
		Deactivate();
	}
	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();

	if (ImGui::Button("Cancel", ImVec2(120, 0)))
	{
		ImGui::CloseCurrentPopup();
		Deactivate();
	}
}

void SE_Create::Activate()
{
	SetActive(true);
}

void SE_Create::Deactivate()
{
	m_Texture = nullptr;
	m_LT = Vec2(0.f, 0.f);
	m_Slice = Vec2(0.f, 0.f);
	m_Offset = Vec2(0.f, 0.f);
	m_BackGround = Vec2(0.f, 0.f);
	m_strName.clear();
	SetActive(false);
}