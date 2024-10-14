#include "pch.h"
#include "AE_Preview.h"

#include <Engine/CTimeMgr.h>

#include "AE_Detail.h"

AE_Preview::AE_Preview()
	: m_CurFrmIdx(0)
	, m_FPS(0.f)
	, m_AccTime(0.f)
	, m_Play(false)
	, m_Finish()
{
}

AE_Preview::~AE_Preview()
{
}

void AE_Preview::Init()
{
}

void AE_Preview::Update()
{
	if (nullptr == GetAnimation() || GetAnimation()->GetSprites().empty())
		return;

	if (m_Finish)
	{
		if (!m_Play)
			return;

		Reset();
	}

	if (nullptr != GetAnimation())
	{
		float MaxTime = 1.f / m_FPS;

		if (m_Play)
			m_AccTime += EngineDT;

		if (MaxTime < m_AccTime)
		{
			m_AccTime -= MaxTime;
			++m_CurFrmIdx;

			if (GetAnimation()->GetMaxFrameCount() <= m_CurFrmIdx)
			{
				--m_CurFrmIdx;
				m_Finish = true;
			}
		}

		m_Sprite = GetAnimation()->GetSprite(m_CurFrmIdx);
	}

	Preview();
}

void AE_Preview::Deactivate()
{
	m_Sprite = nullptr;
	Reset();
}

void AE_Preview::Reset()
{
	m_CurFrmIdx = 0;
	m_AccTime = 0.f;
	m_Finish = false;
}

void AE_Preview::Preview()
{
	if (nullptr == m_Sprite)
		return;

	Ptr<CTexture> pTexture = m_Sprite->GetAtlasTexture();

	// ÀÌ¹ÌÁö	
	ImVec2 uv_min = ImVec2(m_Sprite->GetLeftTopUV().x, m_Sprite->GetLeftTopUV().y);
	ImVec2 uv_max = ImVec2(uv_min.x + m_Sprite->GetSliceUV().x, uv_min.y + m_Sprite->GetSliceUV().y);

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

	Vec2 vResolution = Vec2((float)pTexture->Width(), (float)pTexture->Height());

	ImVec2 BackGroundSize = ImVec2(vResolution.x * m_Sprite->GetBackgroundUV().x, vResolution.y * m_Sprite->GetBackgroundUV().y);
	ImVec2 Offset = ImVec2(m_Sprite->GetOffsetUV().x, m_Sprite->GetOffsetUV().y);

	ImGui::Image(pTexture->GetSRV().Get(), BackGroundSize
		, ImVec2(uv_min.x + Offset.x, uv_min.y + Offset.y)
		, ImVec2(uv_max.x + Offset.x, uv_max.y + Offset.y)
		, tint_col, border_col);
}