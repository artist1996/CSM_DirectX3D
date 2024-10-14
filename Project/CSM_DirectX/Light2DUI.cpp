#include "pch.h"
#include "Light2DUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CLight2D.h>
#include <Engine/CTransform.h>

Light2DUI::Light2DUI()
	: ComponentUI(COMPONENT_TYPE::LIGHT2D)
{
}

Light2DUI::~Light2DUI()
{
}

void Light2DUI::Update()
{
	Title();

	CLight2D* pLight = GetTargetObject()->Light2D();

	// ±¤¿ø Á¾·ù
	LIGHT_TYPE Type = pLight->GetLightType();

	const char* szItems[] = { "DIRECTIONAL", "POINT", "SPOT" };
	const char* combo_preview_items = szItems[(UINT)Type];

	ImGui::Text("Light Type");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(180);

	if (ImGui::BeginCombo("##LightTypeCombo", combo_preview_items))
	{
		for (int i = 0; i < 3; ++i)
		{
			const bool is_selected = ((UINT)Type == i);

			if (ImGui::Selectable(szItems[i], is_selected))
			{
				Type = (LIGHT_TYPE)i;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	pLight->SetLightType(Type);

	// ±¤¿ø »ö»ó Á¤º¸
	const tLightInfo& Info = pLight->GetLightInfo();

	ImGui::Text("Light Color");
	ImGui::SameLine(100);
	ImGui::ColorEdit3("##LightColor", Info.light.Color);

	ImGui::Text("Light Ambient");
	ImGui::SameLine(100);
	ImGui::ColorEdit3("##LightAmbient", Info.light.Ambient);

	// ±¤¿øÀÇ ¹Ý°æ
	// POINT, SPOT

	ImGui::BeginDisabled(LIGHT_TYPE::DIRECTIONAL == Type);

	ImGui::Text("Light Radius");
	ImGui::SameLine(100);
	ImGui::DragFloat("##DragRadius", (float*)&Info.Radius, 0.1f);
	ImGui::EndDisabled();

	ImGui::BeginDisabled(LIGHT_TYPE::SPOT != Type);
	
	float Angle = Info.Angle;
	Angle = (Angle / XM_PI) * 180.f;

	ImGui::Text("Light Angle");
	ImGui::SameLine(100);
	ImGui::DragFloat("##DragAngle", &Angle, 0.1f);

	Angle = (Angle / 180.f) * XM_PI;
	pLight->SetAngle(Angle);
	ImGui::EndDisabled();
}