#include "pch.h"
#include "LandScapeUI.h"

#include <Engine/CLandScape.h>
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
	float fTessLevel = pLandScape->GetTessLevel();
	
	Ptr<CTexture> pHeightMap = pLandScape->GetHeightMap();

	if (ParamUI::DragInt(&iFaceX, 1.f, "FaceX"))
		pLandScape->SetFace(iFaceX, iFaceZ);
	if (ParamUI::DragInt(&iFaceZ, 1.f, "FaceZ"))
		pLandScape->SetFace(iFaceX, iFaceZ);
	if (ParamUI::DragFloat(&fTessLevel, 1.f, "Tesselation Level"))
		pLandScape->SetTessLevel(fTessLevel);
	if (ParamUI::InputTexture(pHeightMap, "Height Map"))
		pLandScape->SetHeightMap(pHeightMap);
}