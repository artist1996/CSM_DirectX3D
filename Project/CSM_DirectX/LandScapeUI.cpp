#include "pch.h"
#include "LandScapeUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CLandScape.h>
#include "ListUI.h"
#include "ParamUI.h"

LandScapeUI::LandScapeUI()
	: ComponentUI(COMPONENT_TYPE::LANDSCAPE)
{
}

LandScapeUI::~LandScapeUI()
{
}

void LandScapeUI::Init()
{
}

void LandScapeUI::Update()
{
	Title();

	CLandScape* pLandScape = GetTargetObject()->LandScape();

	int iFaceX = pLandScape->GetFaceX();
	int iFaceZ = pLandScape->GetFaceZ();
	

	if (ParamUI::DragInt(&iFaceX, 1.f, "FaceX"))
		pLandScape->SetFace(iFaceX, iFaceZ);
	if (ParamUI::DragInt(&iFaceZ, 1.f, "FaceZ"))
		pLandScape->SetFace(iFaceX, iFaceZ);
		

	// Mode
	LANDSCAPE_MODE Type = pLandScape->GetMode();

	const char* szItems[] = { "None" ,"HEIGHT MAP", "SPLATING"};
	const char* combo_preview_items = szItems[(UINT)Type];

	ImGui::Text("Mode");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(180);

	if (ImGui::BeginCombo("##LandScapeModeCombo", combo_preview_items))
	{
		for (int i = 0; i < 3; ++i)
		{
			const bool is_selected = ((UINT)Type == i);

			if (ImGui::Selectable(szItems[i], is_selected))
			{
				Type = (LANDSCAPE_MODE)i;
				pLandScape->SetMode(Type);
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	// Brush Scale
	Vec2 vBrushScale = pLandScape->GetBrushScale();

	if (ParamUI::DragVec2(&vBrushScale, 0.05f, "Brush Scale"))
		pLandScape->SetBrushScale(vBrushScale);

	int iWeightIdx = pLandScape->GetWeightIndex();

	if (ParamUI::DragInt(&iWeightIdx, 1, "Weight Index"))
		pLandScape->SetWeightIndex(iWeightIdx);

	// Texture
	Ptr<CTexture> pHeightMap = pLandScape->GetHeightMap();
	Ptr<CTexture> pBrushTex = pLandScape->GetBrushTex();

	ParamUI::InputTexture(pHeightMap, "HeightMap");
	ParamUI::InputTexture(pBrushTex, "Brush Tex", this, (DELEGATE_1)&LandScapeUI::ChangeBrushIndex);
}

void LandScapeUI::ChangeBrushIndex(DWORD_PTR _Param)
{
	// 마지막으로 선택한 항목이 무엇인지 ListUI 를 통해서 알아냄
	ListUI* pListUI = (ListUI*)_Param;
	string strName = pListUI->GetSelectName();

	CLandScape* pLandScape = GetTargetObject()->LandScape();

	if ("None" == strName)
	{
		pLandScape->SetHeightMap(nullptr);
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);

	assert(pTex.Get());

	pLandScape->SetBurshIndex(pTex);
}
