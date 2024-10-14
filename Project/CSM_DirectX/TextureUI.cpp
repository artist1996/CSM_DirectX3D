#include "pch.h"
#include "TextureUI.h"

TextureUI::TextureUI()
	: AssetUI(ASSET_TYPE::TEXTURE)
{
}

TextureUI::~TextureUI()
{
}

void TextureUI::Update()
{
	Title();

	Ptr<CTexture> pTexture = (CTexture*)GetAsset().Get();

	// Texture Image
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

	ImGui::Image(pTexture->GetSRV().Get(), ImVec2(300.f,300.f), uv_min, uv_max, tint_col, border_col);

	// Texture Name
	string strName = string(pTexture->GetKey().begin(), pTexture->GetKey().end());

	ImGui::Text("Name");
	ImGui::SameLine(100);
	ImGui::InputText("##TextureName", (char*)strName.c_str(), strName.length(), ImGuiInputTextFlags_ReadOnly);

	// Texture Width, Height
	UINT Width = pTexture->Width();
	UINT Height = pTexture->Height();

	char szBuffer[50] = {};
	sprintf_s(szBuffer, 50, "%d", Width);

	ImGui::Text("Width");
	ImGui::SameLine(100);
	ImGui::InputText("##TextureWidth", szBuffer,50, ImGuiInputTextFlags_ReadOnly);

	sprintf_s(szBuffer, 50, "%d", Height);
	ImGui::Text("Height");
	ImGui::SameLine(100);
	ImGui::InputText("##TextureHeight", szBuffer, 50, ImGuiInputTextFlags_ReadOnly);
}