#include "pch.h"
#include "TransformUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CTransform.h>

TransformUI::TransformUI()
	: ComponentUI(COMPONENT_TYPE::TRANSFORM)
	, m_Height(0.f)
{
}

TransformUI::~TransformUI()
{
}

void TransformUI::Update()
{
	Title();
	m_Height = 0.f;
	m_Height += ImGui::GetItemRectSize().y;

	CTransform* pTrans = GetTargetObject()->Transform();

	if (nullptr == pTrans)
		return;

	Vec3 vPos = pTrans->GetRelativePos();
	Vec3 vScale = pTrans->GetRelativeScale();
	Vec3 vRot = pTrans->GetRelativeRotation();
	bool IS = pTrans->IsIndependentScale();
	vRot = (vRot / XM_PI) * 180.f;

	ImGui::Text("Position");
	ImGui::SameLine(100);
	ImGui::DragFloat3("##Pos", vPos);
	m_Height += ImGui::GetItemRectSize().y;

	ImGui::Text("Scale");
	ImGui::SameLine(100);
	ImGui::DragFloat3("##Scale", vScale);
	m_Height += ImGui::GetItemRectSize().y;

	ImGui::Text("Rotation");
	ImGui::SameLine(100);
	ImGui::DragFloat3("##Rot", vRot, 0.1f);
	m_Height += ImGui::GetItemRectSize().y;

	pTrans->SetRelativePos(vPos);
	pTrans->SetRelativeScale(vScale);

	vRot = (vRot / 180.f) * XM_PI;
	pTrans->SetRelativeRotation(vRot);

	// Independent Scale
	ImGui::Text("Ignore Parent");
	ImGui::SameLine(100);
	if (ImGui::Checkbox("##TransIS", &IS))
	{
		pTrans->SetIndependentScale(IS);
	}
	m_Height += ImGui::GetItemRectSize().y;
	SetChildSize({ 0.f, m_Height + 30.f });
}