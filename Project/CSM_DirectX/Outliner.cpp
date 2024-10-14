#include "pch.h"
#include "Outliner.h"

#include "CEditorMgr.h"
#include "Inspector.h"
#include "TreeUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>


Outliner::Outliner()
	: m_Tree(nullptr)
{
	m_Tree = new TreeUI;
	m_Tree->SetName("OutlinerTree");
	AddChild(m_Tree);
	SetMove(false);
	m_Tree->SetShowRoot(false);
	m_Tree->UseDrag(true);
	m_Tree->UseDrop(true);
	m_Tree->AddClickedDelegate(this, (DELEGATE_1)&Outliner::GameObjectClicked);

	m_Tree->AddSelfDragDropDelegate(this, (DELEGATE_2)&Outliner::GameObjectAddChild);
	m_Tree->AddDropDelegate(this, (DELEGATE_2)&Outliner::DroppedFromOuter);
	m_Tree->SetDropPayLoadName("OutlinerTree");

	RenewLevel();
}

Outliner::~Outliner()
{
}

void Outliner::Update()
{
	if (CLevelMgr::GetInst()->IsLevelChanged())
	{
		RenewLevel();
	}
}

void Outliner::RenewLevel()
{
	m_Tree->Clear();

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (nullptr == pLevel)
		return;

	TreeNode* pRootNode = m_Tree->AddNode(nullptr, "Root");

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pLevel->GetLayer(i);

		const vector<CGameObject*>& vecObjects = pLayer->GetParentObjects();

		for (size_t i = 0; i < vecObjects.size(); ++i)
		{
			AddGameObject(pRootNode, vecObjects[i]);
		}
	}
}

void Outliner::GameObjectClicked(DWORD_PTR _Param)
{
	TreeNode* pNode = (TreeNode*)_Param;
	CGameObject* pObject = (CGameObject*)pNode->GetData();

	if (nullptr == pObject)
		return;
	
	Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
	
	pInspector->SetTargetObject(pObject);
	//ImGui::SetWindowFocus(nullptr);
}

void Outliner::AddGameObject(TreeNode* _Node, CGameObject* _Object)
{
	string strName = string(_Object->GetName().begin(), _Object->GetName().end());

	TreeNode* pObjectNode = m_Tree->AddNode(_Node, strName.c_str(), (DWORD_PTR)_Object);

	const vector<CGameObject*>& vecChild = _Object->GetChildren();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObject(pObjectNode, vecChild[i]);
	}
}

void Outliner::GameObjectAddChild(DWORD_PTR _Param1, DWORD_PTR _Param2)
{
	TreeNode* pDragNode = (TreeNode*)_Param1;
	TreeNode* pDropNode = (TreeNode*)_Param2;
	// UI
	// UI Manager
	// Hover UI Tick <<

	CGameObject* pDragObject = (CGameObject*)pDragNode->GetData();
	CGameObject* pDropObject = nullptr;
	
	if (pDropNode)
	{
		pDropObject = (CGameObject*)pDropNode->GetData();

		if (pDropObject->IsAncestor(pDragObject))
			return;
		
		pDropObject->AddChild(pDragObject);
	}
	else
	{
		if (!pDragObject->GetParent())
			return;
		
		pDragObject->DeregisterChild();
		
		CLevelMgr::GetInst()->GetCurrentLevel()->RegisterAsParent(pDragObject->GetLayerIdx(), pDragObject);
	}
}

void Outliner::DroppedFromOuter(DWORD_PTR _OuterData, DWORD_PTR _DropNode)
{
	TreeNode* pContentNode = *((TreeNode**)_OuterData);
	TreeNode* pDropNode = (TreeNode*)_DropNode;
}
