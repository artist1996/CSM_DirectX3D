#include "pch.h"
#include "SkyBoxUI.h"

#include <Engine/CSkyBox.h>
#include "ParamUI.h"

SkyBoxUI::SkyBoxUI()
	: ComponentUI(COMPONENT_TYPE::SKYBOX)
{
}

SkyBoxUI::~SkyBoxUI()
{
}

void SkyBoxUI::Update()
{
	Title();

	CSkyBox* pSkyBox = GetTargetObject()->SkyBox();

	// SkyBox Type
	SKYBOX_TYPE Type = pSkyBox->GetSkyBoxType();

	const char* szItems[] = { "Sphere", "Cube" };
	const char* combo_preview_items = szItems[(UINT)Type];

	ImGui::Text("SkyBox Type");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(180);

	if (ImGui::BeginCombo("##SkyBoxTypeCombo", combo_preview_items))
	{
		for (int i = 0; i < 2; ++i)
		{
			const bool is_selected = ((UINT)Type == i);

			if (ImGui::Selectable(szItems[i], is_selected))
			{
				Type = (SKYBOX_TYPE)i;
				pSkyBox->SetSkyBoxType(Type);
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	
	Ptr<CTexture> pTex = pSkyBox->GetSkyBoxTexture();
	
	bool bChange = ParamUI::InputTexture(pTex, "TEXTURE", this);

	if(bChange)
		pSkyBox->SetSkyBoxTexture(pTex);

}