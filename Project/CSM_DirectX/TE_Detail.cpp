#include "pch.h"
#include "TE_Detail.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CGameObject.h>

#include "CEditorMgr.h"
#include "TreeUI.h"
#include "ListUI.h"

#include "TE_TextureView.h"
#include "TE_Renderer.h"

TE_Detail::TE_Detail()
	: m_TileSize(0.f, 0.f)
	, m_MaxCol(0)
	, m_MaxRow(0)
{
}

TE_Detail::~TE_Detail()
{
}

void TE_Detail::Init()
{

}

void TE_Detail::Update()
{
	if (!IsActive())
		return;

	TileMapInfo();
}

void TE_Detail::TileMapInfo()
{
	string strKey;

	ImTextureID TexID = nullptr;

	if (nullptr != GetTargetObject())
		strKey = string(GetTargetObject()->GetName().begin(), GetTargetObject()->GetName().end());
		
	ImGui::Text("Name");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(130);
	ImGui::InputText("##TEDetailTexName", (char*)strKey.c_str(), strKey.length(), ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("OutlinerTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			CGameObject* pObject = (CGameObject*)pNode->GetData();
			if (pObject->TileMap())
			{
				SetTargetObject(pObject);
				GetTextureView()->SetTargetObject(pObject);
				GetRenderer()->SetTargetObject(pObject);
			}
		}

		ImGui::EndDragDropTarget();
	}

	bool UseGrid = GetRenderer()->IsGrid();

	ImGui::Text("Grid");
	ImGui::SameLine();
	if (ImGui::Checkbox("##RendererUseGrid", &UseGrid))
	{
		GetRenderer()->UseGrid(UseGrid);
	}
}