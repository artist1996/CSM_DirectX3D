#include "pch.h"
#include "AE_Detail.h"
#include <Engine/CAssetMgr.h>

#include "CEditorMgr.h"
#include "AE_Preview.h"
#include "AE_SpriteView.h"
#include "Content.h"
#include "ListUI.h"
#include "TreeUI.h"

AE_Detail::AE_Detail()
	: m_FPS(0.f)
	, m_CurFrameIdx(0)
	, m_ClearNodes(false)
{
}

AE_Detail::~AE_Detail()
{
}

void AE_Detail::Init()
{
}

void AE_Detail::Update()
{	
	// Animation Info
	ShowInfo();

	if (nullptr == GetAnimation())
		return;

	PlayOrStop();

	SpriteList();
}


void AE_Detail::ShowInfo()
{
	// Animation Name

	string strName;
	if (nullptr != GetAnimation())
		strName = string(GetAnimation()->GetKey().begin(), GetAnimation()->GetKey().end());
	

	ImGui::Text("Name");
	ImGui::SameLine(100);
	ImGui::InputText("##AnimName", (char*)strName.c_str()
					, 255
					, ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::ANIMATION == pAsset->GetAssetType())
			{
				SetAnimation((CAnimation*)pAsset.Get());
				GetPreview()->SetAnimation((CAnimation*)pAsset.Get());
			}
		}


		ImGui::EndDragDropTarget();
	}

	// Animation Size
	int iSize = 0;
	
	if(nullptr != GetAnimation())
		iSize = GetAnimation()->GetMaxFrameCount();

	ImGui::Text("Frame Size");
	ImGui::SameLine(100);
	ImGui::DragInt("##AnimationMaxFrameSize", &iSize, 0.f, 0, 0, nullptr, ImGuiSliderFlags_NoInput);
	
	// 현재 Frame Index
	ImGui::Text("Frame Index");
	ImGui::SameLine(100);
	ImGui::DragInt("##AnimationCurFrameIndex", &m_CurFrameIdx);

	// FPS 설정
	ImGui::Text("FPS");
	ImGui::SameLine(100);
	ImGui::DragFloat("##AnimationFPS", &m_FPS);

	ImGui::Separator();
}

void AE_Detail::AddSprite()
{
	if (ImGui::BeginDragDropTarget())
	{
		Content* pContent = (Content*)CEditorMgr::GetInst()->FindEditorUI("Content");

		TreeUI* pTreeUI = pContent->GetTree();

		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (pTreeUI->IsEmptyDuplicateNodes())
		{
			if (payload)
			{
				TreeNode** ppNode = (TreeNode**)payload->Data;
				TreeNode* pNode = *ppNode;

				Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
				if (ASSET_TYPE::SPRITE == pAsset->GetAssetType())
				{
					m_Sprite = ((CSprite*)pAsset.Get());
					GetAnimation()->AddSprite(m_Sprite);
				}
			}
		}
		
		else
		{
			if (payload)
			{
				TreeNode** ppNode = (TreeNode**)payload->Data;
				size_t NodeCount = payload->DataSize / sizeof(TreeNode*);

				for (size_t i = 0; i < NodeCount; ++i)
				{
					TreeNode* pNode = ppNode[i];

					Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();

					if (ASSET_TYPE::SPRITE == pAsset->GetAssetType())
					{
						m_Sprite = ((CSprite*)pAsset.Get());
						GetAnimation()->AddSprite(m_Sprite);
						m_ClearNodes = true;
					}
				}
			}
		}

		ImGui::EndDragDropTarget();
	}

	if (ImGui::Button("ADD SPRITE", ImVec2(88.f, 20.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("Sprite");
		vector<string> vecSpriteNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::SPRITE, vecSpriteNames);
		pListUI->AddList(vecSpriteNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&AE_Detail::SelectSprite);
		pListUI->SetActive(true);
	}
}

void AE_Detail::PlayOrStop()
{
	if (ImGui::Button("PLAY", ImVec2(40.f, 25.f)))
	{
		GetPreview()->Play();
		GetPreview()->SetFPS(m_FPS);
	}

	ImGui::SameLine();

	if (ImGui::Button("STOP", ImVec2(40.f, 25.f)))
	{
		GetPreview()->Stop();
	}

	ImGui::Separator();
}

void AE_Detail::SpriteList()
{
	const vector<Ptr<CSprite>>& vecSprites = GetAnimation()->GetSprites();

	string strName;

	if (nullptr != m_Sprite)
		strName = string(m_Sprite->GetKey().begin(), m_Sprite->GetKey().end());

	static int item_current_idx = 0; // Here we store our selection data as an index.
	if (ImGui::BeginListBox("##SpritesNames"))
	{
		if (!vecSprites.empty())
		{
			for (int i = 0; i < vecSprites.size(); i++)
			{
				const bool is_selected = (item_current_idx == i);
				if (ImGui::Selectable(string(vecSprites[i]->GetKey().begin(), vecSprites[i]->GetKey().end()).c_str(), is_selected))
					item_current_idx = i;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
				{
					m_Sprite = CAssetMgr::GetInst()->FindAsset<CSprite>(vecSprites[i]->GetKey());
					m_CurFrameIdx = item_current_idx;
					GetSV()->SetSprite(vecSprites[m_CurFrameIdx]);
					ImGui::SetItemDefaultFocus();
				}
			}
		}

		ImGui::EndListBox();
	}
	ImGui::SameLine();
	ImGui::BeginGroup();
	AddSprite();
	
	ImGui::Button("Insert", ImVec2(88.f, 20.f));

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::SPRITE == pAsset->GetAssetType())
			{
				GetAnimation()->Insert(m_CurFrameIdx, (CSprite*)pAsset.Get());
			}
		}	

		ImGui::EndDragDropTarget();
	}
	
	if (ImGui::Button("Delete", ImVec2(88.f, 20.f)))
	{
		GetAnimation()->erase(m_CurFrameIdx);
	}

	ImGui::EndGroup();
}

void AE_Detail::SelectSprite(DWORD_PTR _Param)
{
	ListUI* pListUI = (ListUI*)_Param;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		m_Sprite = nullptr;
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(strAssetName);

	assert(pSprite.Get());
	
	m_Sprite = pSprite;
	GetAnimation()->AddSprite(m_Sprite);
}

void AE_Detail::Deactivate()
{
	m_Sprite = nullptr;
}