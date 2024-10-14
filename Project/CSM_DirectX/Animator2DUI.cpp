#include "pch.h"
#include "Animator2DUI.h"
#include "CEditorMgr.h"

#include <Engine/CGameObject.h>
#include <Engine/CAssetMgr.h>
#include <Engine/CSprite.h>
#include <Engine/CAnimation.h>
#include <Engine/CAnimator2D.h>

#include "Content.h"
#include "ListUI.h"
#include "TreeUI.h"

Animator2DUI::Animator2DUI()
	: ComponentUI(COMPONENT_TYPE::ANIMATOR2D)
	, m_UIHeight(0)
	, m_Idx(0)
	, m_SelectedIdx(0)
{
}

Animator2DUI::~Animator2DUI()
{
}

void Animator2DUI::Update()
{
	Title();
	m_UIHeight = 0;

	m_UIHeight += (int)ImGui::GetItemRectSize().y;
	CGameObject* pObject = GetTargetObject();

	CAnimator2D* pAnimator2D = pObject->Animator2D();
		
	if ((int)pAnimator2D->GetAnimationsSize() <= m_Idx)
	{
		m_Idx = (int)pAnimator2D->GetAnimationsSize() - 1;
	}

	if (m_Idx <= 0)
		m_Idx = 0;

	Ptr<CAnimation> pAnimation = pAnimator2D->GetCurAnimation();

	ImGui::Text("Animation");
	ImGui::SameLine();
	string strName;
	if (nullptr != pAnimation)
		strName = string(pAnimation->GetKey().begin(), pAnimation->GetKey().end());
	else
		strName = "";

	ImGui::SetNextItemWidth(183.f);
	ImGui::InputText("##CurAnimationName", (char*)strName.c_str(), strName.length(), ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* Payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (Payload)
		{
			TreeNode* pNode = *((TreeNode**)Payload->Data);
			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();

			if (ASSET_TYPE::ANIMATION == pAsset->GetAssetType())
			{
				vector<Ptr<CAnimation>> pAnimation = pAnimator2D->GetAnimations();

				if (0 == pAnimation.size())
				{
					pAnimator2D->AddAnimation(0, (CAnimation*)pAsset.Get());
				}
				else
				{
					pAnimator2D->AddAnimation((int)pAnimation.size(), (CAnimation*)pAsset.Get());
				}
			}
		}
		
		ImGui::EndDragDropTarget();
	}


	if (ImGui::Button("##CurAnimationBtn", ImVec2(20.f,20.f)))
	{
		// ListUI 활성화
		ListUI* pList = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pList->SetName("Animation");
		pList->AddDelegate(this, (DELEGATE_1)&Animator2DUI::SelectFlipBook);

		// AssetMgr 로 부터 Mesh Key 값 들고오기
		
		vector<string> vecAnimationNames;

		for (size_t i = 0; i < pAnimator2D->GetAnimations().size(); ++i)
		{
			if (nullptr == pAnimator2D->GetAnimations()[i])
				continue;

			vecAnimationNames.push_back(string(pAnimator2D->GetAnimations()[i]->GetKey().begin(), pAnimator2D->GetAnimations()[i]->GetKey().end()));
		}
		
		pList->AddList(vecAnimationNames);
		pList->SetActive(true);
	}

	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	const vector<Ptr<CAnimation>>& vecAnimations = pAnimator2D->GetAnimations();

	static int item_current_idx = 0; // Here we store our selection data as an index.
	if (ImGui::BeginListBox("##AnimationsListBox"))
	{
		if (!vecAnimations.empty())
		{
			for (int i = 0; i < vecAnimations.size(); i++)
			{
				const bool is_selected = (item_current_idx == i);
				if (ImGui::Selectable(string(vecAnimations[i]->GetKey().begin(), vecAnimations[i]->GetKey().end()).c_str(), is_selected))
					item_current_idx = i;

				// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
				if (is_selected)
				{
					Ptr<CAnimation> pAnimation = CAssetMgr::GetInst()->FindAsset<CAnimation>(vecAnimations[i]->GetKey());
					m_SelectedIdx = i;
					//pAnimator2D->SetCurAnimation(vecAnimations[i]);
					ImGui::SetItemDefaultFocus();
				}
			}
		}

		ImGui::EndListBox();
	}

	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* Payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (Payload)
		{
			TreeNode* pNode = *((TreeNode**)Payload->Data);
			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();

			if (ASSET_TYPE::ANIMATION == pAsset->GetAssetType())
			{
				vector<Ptr<CAnimation>> pAnimation = pAnimator2D->GetAnimations();

				if (0 == pAnimation.size())
				{
					pAnimator2D->AddAnimation(0, (CAnimation*)pAsset.Get());
				}
				else
				{
					pAnimator2D->AddAnimation((int)pAnimation.size(), (CAnimation*)pAsset.Get());
					//pAnimator2D->SetCurAnimation((CAnimation*)pAsset.Get());
				}
			}
		}

		ImGui::EndDragDropTarget();
	}

	// Erase
	ImGui::SameLine();

	if(ImGui::Button("DELETE", ImVec2(65.f, 18.f)))
	{
		pAnimator2D->erase(m_SelectedIdx);
		pAnimator2D->SetCurAnimation(nullptr);
	}
	
	if (ImGui::Button("PLAY##Animator2DUI", ImVec2(65.f, 18.f)))
	{
		pAnimator2D->Play(m_SelectedIdx, 1.f, true);
	}

	if (ImGui::Button("STOP##Animator2DUI", ImVec2(65.f, 18.f)))
	{
		pAnimator2D->Stop();
	}

	// Cur Sprite
	//Ptr<CSprite> pSprite = pAnimator2D->GetCurSprite();
	//
	//if (nullptr == pSprite)
	//{
	//	SetChildSize(ImVec2(0.f, (float)m_UIHeight + 10.f));
	//	return;
	//}
	//
	//ImGui::Text("Cur Sprite");
	//ImGui::SameLine(100);
	//m_UIHeight += (int)ImGui::GetItemRectSize().y;
	//
	//// Cur Sprite Name
	//strName = string(pSprite->GetKey().begin(), pSprite->GetKey().end());
	//ImGui::InputText("##CurSprite Name", (char*)strName.c_str(), strName.length(), ImGuiInputTextFlags_ReadOnly);
	//m_UIHeight += (int)ImGui::GetItemRectSize().y;
	//
	//// Cur Sprite Image
	//ImVec2 uv_min = ImVec2(pSprite->GetLeftTopUV().x, pSprite->GetLeftTopUV().y);
	//ImVec2 uv_max = ImVec2(uv_min.x + pSprite->GetSliceUV().x, uv_min.y + pSprite->GetSliceUV().y);
	//
	//ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	//ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
	//
	//ImGui::Image(pSprite->GetAtlasTexture()->GetSRV().Get(), ImVec2(150.f, 150.f), uv_min, uv_max, tint_col, border_col);
	//m_UIHeight += (int)ImGui::GetItemRectSize().y;
	//// Cur Frame Index
	//int CurIndex = pAnimator2D->GetCurFrameIndex();
	//
	//ImGui::Text("Frame Index");
	//ImGui::SameLine(100);
	//ImGui::DragInt("##FrameIndex", &CurIndex);
	//m_UIHeight += (int)ImGui::GetItemRectSize().y;
	//
	//float FPS = pAnimator2D->GetFPS();
	//
	//ImGui::Text("FPS");
	//ImGui::SameLine(100);
	//ImGui::DragFloat("##Animation FPS", &FPS);
	//m_UIHeight += (int)ImGui::GetItemRectSize().y;
	//
	//pAnimator2D->SetFPS(FPS);


	SetChildSize(ImVec2(0.f, (float)m_UIHeight + 20.f));
}

void Animator2DUI::SelectFlipBook(DWORD_PTR _ListUI)
{
	ListUI* pList = (ListUI*)_ListUI;
	string strName = pList->GetSelectName();
	wstring strAnimationName = wstring(strName.begin(), strName.end());
	Ptr<CAnimation> pAnimation = CAssetMgr::GetInst()->FindAsset<CAnimation>(strAnimationName);

	assert(pAnimation.Get());
	CAnimator2D* pAnimator2D = GetTargetObject()->Animator2D();
	pAnimator2D->SetCurAnimation(pAnimation);
	pAnimator2D->Reset();
}