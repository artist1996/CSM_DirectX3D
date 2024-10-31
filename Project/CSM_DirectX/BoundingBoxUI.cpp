#include "pch.h"
#include "BoundingBoxUI.h"

#include <Engine/CBoundingBox.h>

BoundingBoxUI::BoundingBoxUI()
	: ComponentUI(COMPONENT_TYPE::BOUNDINGBOX)
{
}

BoundingBoxUI::~BoundingBoxUI()
{
}

void BoundingBoxUI::Init()
{
}

void BoundingBoxUI::Update()
{
	Title();

	CBoundingBox* pBoundingBox = GetTargetObject()->BoundingBox();

	float fRadius = pBoundingBox->GetRadius();
	float fOffset = pBoundingBox->GetOffset();

	ImGui::Text("Radius");
	ImGui::SameLine(100);
	ImGui::DragFloat("##BoundingBoxRadius", &fRadius, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Offset");
	ImGui::SameLine(100);
	if (ImGui::DragFloat("##BoundingBoxOffset", &fOffset))
	{
		pBoundingBox->SetOffset(fOffset);
	}
}