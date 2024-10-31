#include "pch.h"
#include "DecalUI.h"

#include <Engine/CDecal.h>

#include "ParamUI.h"

DecalUI::DecalUI()
	: ComponentUI(COMPONENT_TYPE::DECAL)
{
}

DecalUI::~DecalUI()
{
}

void DecalUI::Init()
{
}

void DecalUI::Update()
{
	Title();

	CDecal* pDecal = GetTargetObject()->Decal();

	Ptr<CTexture> pTex = pDecal->GetDecalTexture();

	bool bDecalTexChange = ParamUI::InputTexture(pTex, "Decal Tex", this);

	if (bDecalTexChange)
		pDecal->SetDecalTexture(pTex);

	Ptr<CTexture> pEmissiveTex = pDecal->GetEmisiiveTexture();

	bool bEmissiveTexChange = ParamUI::InputTexture(pEmissiveTex, "Emissive Tex", this);

	if (bEmissiveTexChange)
		pDecal->SetEmissiveTexture(pEmissiveTex);
}