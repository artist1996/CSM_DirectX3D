#include "pch.h"
#include "TileMapUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CTileMap.h>

#include "CEditorMgr.h"
#include "TreeUI.h"
#include "TileMapEditor.h"

TileMapUI::TileMapUI()
	: ComponentUI(COMPONENT_TYPE::TILEMAP)
	, m_UIHeight(0)
	, m_CheckEditor(false)
{
}

TileMapUI::~TileMapUI()
{
}


void TileMapUI::Update()
{
	m_UIHeight = 0;
	Title();
	m_UIHeight += (UINT)ImGui::GetItemRectSize().y;

	CTileMap* pTileMap = GetTargetObject()->TileMap();

	string strTexKey;

	if(nullptr != pTileMap->GetTexture())
		strTexKey = string(pTileMap->GetTexture()->GetKey().begin(), pTileMap->GetTexture()->GetKey().end());


	ImGui::Text("Texture");
	ImGui::SameLine(92.f);
	ImGui::InputText("##TileMapTexKey", (char*)strTexKey.c_str(), strTexKey.length(), ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;
	
			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::TEXTURE == pAsset->GetAssetType())
			{
				pTileMap->SetAtlasTexture((CTexture*)pAsset.Get());
				Ptr<CTexture> pTex = (CTexture*)pAsset.Get();
			}
		}
	
		ImGui::EndDragDropTarget();
	}

	m_UIHeight += (UINT)ImGui::GetItemRectSize().y;

	m_UIHeight += (UINT)ImGui::GetItemRectSize().y + 20;
	
	int MaxIdx = pTileMap->GetTileMaxIdx();

	ImGui::Text("Index");
	ImGui::SameLine(92.f);
	ImGui::InputInt("##TileMaxIndex", &MaxIdx, 0, ImGuiInputTextFlags_ReadOnly);
	m_UIHeight += (UINT)ImGui::GetItemRectSize().y;

	Vec2 TileSize = pTileMap->GetTileSize();
	ImGui::Text("TileSize");
	ImGui::SameLine(92.f);
	ImGui::DragFloat2("##TileSize", (float*)&TileSize);
	pTileMap->SetTileSize(TileSize);
	m_UIHeight += (UINT)ImGui::GetItemRectSize().y;

	Vec2 TexTileSize = pTileMap->GetTextureTileSize();
	ImGui::Text("Tex TileSize");
	ImGui::SameLine();
	ImGui::DragFloat2("##TileMapTexTileSize", (float*)&TexTileSize);
	m_UIHeight += (UINT)ImGui::GetItemRectSize().y;
	pTileMap->SetAtlasTileSize(TexTileSize);

	int Row = pTileMap->GetRow();
	int Col = pTileMap->GetCol();

	ImGui::Text("Tile Row");
	ImGui::SameLine(92.f);
	ImGui::InputInt("##TileMapRow", &Row);
	m_UIHeight += (UINT)ImGui::GetItemRectSize().y;
	pTileMap->SetRowCol(Row, Col);

	ImGui::Text("Tile Col");
	ImGui::SameLine(92.f);
	ImGui::InputInt("##TileMapCol", &Col);
	m_UIHeight += (UINT)ImGui::GetItemRectSize().y;
	pTileMap->SetRowCol(Row, Col);

	SetChildSize(ImVec2(0.f, (float)m_UIHeight + 10.f));

	
	ImGui::Text("Editor");
	ImGui::SameLine(92.f);

	if (ImGui::Checkbox("##TileMapEditorCheck", &m_CheckEditor))
	{
		TileMapEditor* pTileMapEditor = (TileMapEditor*)CEditorMgr::GetInst()->FindEditorUI("TileMap Editor");
		pTileMapEditor->SetActive(m_CheckEditor);
		pTileMapEditor->SetTargetObject(GetTargetObject());
	}
}