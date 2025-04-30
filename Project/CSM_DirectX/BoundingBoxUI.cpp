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
	return;
	Title();

	//CBoundingBox* pBoundingBox = GetTargetObject()->BoundingBox();

	float fRadius = GetTargetObject()->BoundingBox()->GetRadius();
	float fOffset = GetTargetObject()->BoundingBox()->GetOffset();

	ImGui::Text("Radius");
	ImGui::SameLine(100);
	ImGui::DragFloat("##BoundingBoxRadius", &fRadius, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Offset");
	ImGui::SameLine(100);
	if (ImGui::DragFloat("##BoundingBoxOffset", &fOffset))
	{
		GetTargetObject()->BoundingBox()->SetOffset(fOffset);
	}

	bool& IsRender = GetTargetObject()->BoundingBox()->IsDebugRender();
	ImGui::Text("Debug Render");
	ImGui::SameLine(100);
	ImGui::Checkbox("##BoundingBoxDebugRender", &IsRender);

}