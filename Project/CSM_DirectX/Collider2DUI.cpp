#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/CCollider2D.h>

Collider2DUI::Collider2DUI()
	: ComponentUI(COMPONENT_TYPE::COLLIDER2D)
	, m_IS(false)
{
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::Update()
{
	Title();

	CCollider2D* pCollider2D = GetTargetObject()->Collider2D();
	Vec3 vOffset = pCollider2D->GetOffset();
	Vec3 vScale = pCollider2D->GetScale();
	m_IS = pCollider2D->IsIndependentScale();

	// Offset
	ImGui::Text("Offset");
	ImGui::SameLine(100);
	if (ImGui::DragFloat3("##Collider2D Offset", vOffset))
	{
		pCollider2D->SetOffset(vOffset);
	}

	// Scale
	ImGui::Text("Scale");
	ImGui::SameLine(100);
	if (ImGui::DragFloat3("##Collider2D Scale", vScale))
	{
		pCollider2D->SetScale(vScale);
	}

	// Independent Scale
	ImGui::Text("Independent Scale");
	ImGui::SameLine(130);
	if (ImGui::Checkbox("##Collider2D Independent Scale", &m_IS))
	{
		pCollider2D->SetIndependentScale(m_IS);
	}
}