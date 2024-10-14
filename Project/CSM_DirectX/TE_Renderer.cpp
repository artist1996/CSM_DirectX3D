#include "pch.h"
#include "TE_Renderer.h"

#include <Engine/CKeyMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CTileMap.h>

TE_Renderer::TE_Renderer()
	: m_TileIdx(0)
	, m_TileImgIdx(0)
	, m_WheelScale(1.5f)
	, m_Ratio(1.f)
	, m_UseGrid(true)
{
	SetHorizontalScrollBar(true);
}

TE_Renderer::~TE_Renderer()
{
}

void TE_Renderer::Init()
{
}

void TE_Renderer::Update()
{
	if (!IsActive() || !GetTargetObject())
		return;

	CTileMap* pTileMap = GetTargetObject()->TileMap();

	m_Ratio = (pTileMap->GetTexture()->Width() * m_WheelScale) / pTileMap->GetTexture()->Width();

	if (ImGui::IsWindowFocused())
		WheelCheck();

	RenderTileMap();
}

void TE_Renderer::WheelCheck()
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

void TE_Renderer::RenderGrid()
{
	CTileMap* pTileMap = GetTargetObject()->TileMap();
}

void TE_Renderer::RenderTileMap()
{
	CTileMap* pTileMap = GetTargetObject()->TileMap();
	
	// Row, Col
	int Row = pTileMap->GetRow();
	int Col = pTileMap->GetCol();
	int TexMaxRow = pTileMap->GetTextureMaxRow();
	int TexMaxCol = pTileMap->GetTextureMaxCol();

	// TileMap Texture
	Ptr<CTexture> pTexture = pTileMap->GetTexture();

	// TileSize
	Vec2 TileSize = pTileMap->GetTileSize() / m_Ratio;

	// Texture TileSize, Slice UV
	Vec2 TexTileSize = pTileMap->GetTextureTileSize();
	Vec2 SliceUV = pTileMap->GetSliceUV();

	// Texture Width, Height
	UINT Width = pTexture->Width();
	UINT Height = pTexture->Height();
	Vec2 TileMapSize = Vec2(Row, Col) * TileSize;

	ImVec2 canvasPos = ImGui::GetCursorScreenPos();

	ImDrawList* drawList = ImGui::GetWindowDrawList();

	for (int i = 0; i < Row; ++i)
	{	
		for (int j = 0; j < Col; ++j)
		{
			m_TileIdx = Col * i + j;
			
			int ImgIdx = pTileMap->GetTileInfoIdx(m_TileIdx);

			if (-1 != ImgIdx)
			{
				m_TilePos = ImVec2(canvasPos.x + j * TileSize.x, canvasPos.y + i * TileSize.y);
			 	
				UINT TileImgRow = ImgIdx / TexMaxCol;
				UINT TileImgCol = ImgIdx % TexMaxCol;

				Vec2 vCurColRow = Vec2(TileImgCol, TileImgRow);

				// 타일의 UV 좌표 계산
				m_uvMin = ImVec2(vCurColRow.x * SliceUV.x, vCurColRow.y * SliceUV.y);
				m_uvMax = ImVec2(m_uvMin.x + SliceUV.x, m_uvMin.y + SliceUV.y);

				drawList->AddImage(pTexture->GetSRV().Get(), m_TilePos, ImVec2(m_TilePos.x + TileSize.x, m_TilePos.y + TileSize.y), m_uvMin, m_uvMax);

				if (m_UseGrid)
				{
					drawList->AddLine(m_TilePos,
									  ImVec2(m_TilePos.x + TileSize.x, m_TilePos.y),
									  ImGui::GetColorU32(ImVec4(0.f, 1.f, 0.f, 1.f)), 1.f);
					drawList->AddLine(m_TilePos,
									  ImVec2(m_TilePos.x, m_TilePos.y + TileSize.y),
									  ImGui::GetColorU32(ImVec4(0.f, 1.f, 0.f, 1.f)), 1.f);

					// 마지막 열의 오른쪽 경계선
					if (j == Col - 1)
					{
						drawList->AddLine(ImVec2(m_TilePos.x + TileSize.x, m_TilePos.y),
							ImVec2(m_TilePos.x + TileSize.x, m_TilePos.y + TileSize.y),
							ImGui::GetColorU32(ImVec4(0.f, 1.f, 0.f, 1.f)), 1.f);
					}

					// 마지막 줄의 아래쪽 경계선
					if (i == Row - 1)
					{
						drawList->AddLine(ImVec2(m_TilePos.x, m_TilePos.y + TileSize.y),
							ImVec2(m_TilePos.x + TileSize.x, m_TilePos.y + TileSize.y),
							ImGui::GetColorU32(ImVec4(0.f, 1.f, 0.f, 1.f)), 1.f);
					}
				}			
			}
		}
	}

	if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
	{
		ImVec2 MousePos = ImGui::GetMousePos();
		ImVec2 vDiff = ImVec2(MousePos.x - canvasPos.x, MousePos.y - canvasPos.y);
		if (vDiff.x >= 0 && vDiff.y >= 0 && vDiff.x < Col * TileSize.x && vDiff.y < Row * TileSize.y)
		{
			int ClickCol = (int)(vDiff.x / TileSize.x);
			int ClickRow = (int)(vDiff.y / TileSize.y);

			m_TileIdx = Col * ClickRow + ClickCol;

			pTileMap->SetTileIdx(m_TileIdx, m_TileImgIdx);
		}
	}
}