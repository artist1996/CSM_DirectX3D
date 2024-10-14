#include "pch.h"
#include "AssetUI.h"

AssetUI::AssetUI(ASSET_TYPE _Type)
	: m_Type(_Type)
	, m_Asset(nullptr)
{
}

AssetUI::~AssetUI()
{
}

void AssetUI::SetAsset(Ptr<CAsset> _Asset)
{
	m_Asset = nullptr;

	if (nullptr == _Asset || m_Type != _Asset->GetAssetType())
	{
		SetActive(false);
	}
	else
	{
		m_Asset = _Asset;
		SetActive(true);
	}
}

void AssetUI::Title()
{
	ImGui::PushID((int)m_Type);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.7f, 0.3f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.7f, 0.3f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.7f, 0.3f, 0.5f));

	ImGui::Button(ToString(m_Type));

	ImGui::PopStyleColor(3);
	ImGui::PopID();
}

void AssetUI::OutputAssetName()
{
	Ptr<CAsset> pAsset = GetAsset();

	string strName = string(pAsset->GetKey().begin(), pAsset->GetKey().end());

	ImGui::Text("Name");
	ImGui::SameLine(100);
	ImGui::InputText("##AssetName", (char*)strName.c_str(), strName.length(), ImGuiInputTextFlags_ReadOnly);
}