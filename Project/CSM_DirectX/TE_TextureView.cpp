#include "pch.h"
#include "TE_TextureView.h"

#include <Engine/CGameObject.h>
#include <Engine/CTileMap.h>

#include "TE_Renderer.h"

TE_TextureView::TE_TextureView()
	: m_ClickCol(0)
	, m_ClickRow(0)
	, m_WheelScale(0.5f)
	, m_Ratio(0.f)
	, m_WidthSize(0.f)
{
}

TE_TextureView::~TE_TextureView()
{
}

void TE_TextureView::Init()
{
}

void TE_TextureView::Update()
{
	if (nullptr == GetTargetObject() || !IsActive())
		return;

	CTileMap* pTileMap = GetTargetObject()->TileMap();

	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);

	Vec2 vResolution = Vec2((float)pTileMap->GetTexture()->Width(), (float)pTileMap->GetTexture()->Height());

	m_WidthSize = (float)vResolution.x;
	
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

	m_Ratio = (m_WidthSize * m_WheelScale) / pTileMap->GetTexture()->Width();


	ImGui::Image(pTileMap->GetTexture()->GetSRV().Get(), ImVec2((m_WidthSize * m_WheelScale), pTileMap->GetTexture()->Height() * m_Ratio)
		, uv_min
		, uv_max
		, tint_col, border_col);

	if (ImGui::IsWindowFocused())
		WheelCheck();

	ClickedCheck();

	DrawRect();
}

void TE_TextureView::WheelCheck()
{
	if (0 < ImGui::GetIO().MouseWheel)
		m_WheelScale += 0.1f;
	if (0 > ImGui::GetIO().MouseWheel)
		m_WheelScale -= 0.1f;
	if (500.f < m_WheelScale)
		m_WheelScale = 500.f;
	if (0.1f > m_WheelScale)
		m_WheelScale = 0.1f;
}


void TE_TextureView::ClickedCheck()
{
	CTileMap* pTileMap = GetTargetObject()->TileMap();
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{		
		ImVec2 MousePos = ImGui::GetMousePos();
		m_ImageRectMin = ImGui::GetItemRectMin();
		ImVec2 TileSize = ImVec2(pTileMap->GetTileSize().x, pTileMap->GetTileSize().y);

		m_vDiff = ImVec2(MousePos.x - m_ImageRectMin.x, MousePos.y - m_ImageRectMin.y);
		int Col = pTileMap->GetTextureMaxCol();
		int Row = pTileMap->GetTextureMaxRow();

		if (m_vDiff.x >= 0 && m_vDiff.y >= 0 && m_vDiff.x < Col * TileSize.x && m_vDiff.y < Row * TileSize.y)
		{
			m_vDiff.x = m_vDiff.x / m_Ratio;
			m_vDiff.y = m_vDiff.y / m_Ratio;

			float RatioTileSizeX = TileSize.x / m_Ratio;
			float RatioTileSizeY = TileSize.y / m_Ratio;

			int ClickCol = int(m_vDiff.x / TileSize.x);
			int ClickRow = int(m_vDiff.y / TileSize.y);

			int TileIdx = Col * ClickRow + ClickCol;

			m_ClickCol = ClickCol;
			m_ClickRow = ClickRow;

			GetRenderer()->SetImgIdx(TileIdx);
		}
	}
}

void TE_TextureView::DrawRect()
{
	CTileMap* pTileMap = GetTargetObject()->TileMap();
	
	// 클릭한 타일의 좌상단 좌표
	ImVec2 LT = ImVec2(m_ImageRectMin.x + ((float)m_ClickCol * (pTileMap->GetTextureTileSize().x) * m_Ratio)
					 , m_ImageRectMin.y + ((float)m_ClickRow * (pTileMap->GetTextureTileSize().y) * m_Ratio));
	
	
	// 클릭한 타일의 우하단 좌표
	ImVec2 RB = ImVec2(LT.x + pTileMap->GetTextureTileSize().x * m_Ratio
					 , LT.y + pTileMap->GetTextureTileSize().y * m_Ratio);


	ImVec2 RectMax = ImGui::GetItemRectMax();
	
	// 사각형을 그리기
	if(m_ImageRectMin.x <= LT.x && m_ImageRectMin.y <= LT.y && RectMax.x >= RB.x && RectMax.y >= RB.y)
		ImGui::GetWindowDrawList()->AddRect(LT, RB, ImGui::GetColorU32(ImVec4(0.f, 1.f, 0.f, 1.f)), 0.f, 0, 2.f);
}