#include "pch.h"
#include "AE_SpriteView.h"

AE_SpriteView::AE_SpriteView()
	: m_Sprite(nullptr)
{
	SetActive(false);
}

AE_SpriteView::~AE_SpriteView()
{
}

void AE_SpriteView::Init()
{
}

void AE_SpriteView::Update()
{
	if (nullptr == m_Sprite || !IsActive())
		return;

	Ptr<CTexture> pTexture = m_Sprite->GetAtlasTexture();

	if (nullptr == pTexture)
		return;
	Vec2 vLT = m_Sprite->GetLeftTopUV();
	Vec2 vSlice = m_Sprite->GetSliceUV();
	ImVec2 uv_min = ImVec2(vLT.x, vLT.y);
	ImVec2 uv_max = ImVec2(vLT.x + vSlice.x, vLT.y + vSlice.y);

	Vec2 vResolution = Vec2((float)pTexture->Width(), (float)pTexture->Height());

	ImVec2 BackGroundSize = ImVec2(m_Sprite->GetBackgroundUV().x * vResolution.x, m_Sprite->GetBackgroundUV().y * vResolution.y);
	ImVec2 Offset = ImVec2(m_Sprite->GetOffsetUV().x, m_Sprite->GetOffsetUV().y);

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

	ImGui::Image(pTexture->GetSRV().Get(), BackGroundSize
		, ImVec2(uv_min.x - Offset.x, uv_min.y - Offset.y)
		, ImVec2(uv_max.x - Offset.x, uv_max.y - Offset.y)
		, tint_col, border_col);
}

void AE_SpriteView::Activate()
{
}

void AE_SpriteView::Deactivate()
{
	m_Sprite = nullptr;
}
