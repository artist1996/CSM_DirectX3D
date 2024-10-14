#include "pch.h"
#include "SpriteUI.h"

SpriteUI::SpriteUI()
	: AssetUI(ASSET_TYPE::SPRITE)
{
}

SpriteUI::~SpriteUI()
{
}

void SpriteUI::Update()
{
	Title();

	// Sprite Image

	Ptr<CSprite> pSprite = (CSprite*)GetAsset().Get();

	ImVec2 uv_min = ImVec2(pSprite->GetLeftTopUV().x, pSprite->GetLeftTopUV().y);
	ImVec2 uv_max = ImVec2(uv_min.x + pSprite->GetSliceUV().x, uv_min.y + pSprite->GetSliceUV().y);

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

	ImGui::Image(pSprite->GetAtlasTexture()->GetSRV().Get(), ImVec2(150.f, 150.f), uv_min, uv_max, tint_col, border_col);

	OutputAssetName();

	// LeftTop
	Vec2 vLeftTopUV = pSprite->GetLeftTopUV();
	Vec2 vResolution = Vec2((float)pSprite->GetAtlasTexture()->Width(), (float)pSprite->GetAtlasTexture()->Height());
	Vec2 vLeftTop = vResolution * vLeftTopUV;

	ImGui::Text("LeftTop");
	ImGui::SameLine(100);
	ImGui::InputFloat2("##LeftTop", (float*)&vLeftTop);

	// Slice
	Vec2 vSliceUV = pSprite->GetSliceUV();
	Vec2 vSlice = vResolution * vSliceUV;
	
	ImGui::Text("Slice");
	ImGui::SameLine(100);
	ImGui::InputFloat2("##Slice", (float*)&vSlice);

	// Offset
	Vec2 vOffsetUV = pSprite->GetOffsetUV();
	Vec2 vOffset = vResolution * vOffsetUV;

	ImGui::Text("Offset");
	ImGui::SameLine(100);
	ImGui::InputFloat2("##Offset", (float*)&vOffset);

	// Background
	Vec2 vBackgroundUV = pSprite->GetBackgroundUV();
	Vec2 vBackground = vResolution * vBackgroundUV;

	ImGui::Text("Background");
	ImGui::SameLine(100);
	ImGui::InputFloat2("##Background", (float*)&vBackground);

	pSprite->SetLeftTopUV(vLeftTop);
	pSprite->SetSliceUV(vSlice);
	pSprite->SetOffsetUV(vOffset);
	pSprite->SetBackgroundUV(vBackground);
}