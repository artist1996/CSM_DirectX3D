#include "pch.h"
#include "RigidbodyUI.h"

#include <Engine/CRigidbody.h>

RigidbodyUI::RigidbodyUI()
	: ComponentUI(COMPONENT_TYPE::RIGIDBODY)
{
}

RigidbodyUI::~RigidbodyUI()
{
}

void RigidbodyUI::Update()
{
    Title();

    m_UIHeight = 0;
    
    CRigidbody* pRB = GetTargetObject()->Rigidbody();

    float Mass = pRB->GetMass();
    float JumpSpeed= pRB->GetJumpSpeed();
    float MaxGravitySpeed = pRB->GetMaxGravitySpeed();
    bool  bGround = pRB->IsGround();
    bool  bUseGravity = pRB->UseGravity();

    ImGui::Text("Mass");
    ImGui::SameLine(100);
    ImGui::DragFloat("##Rigidbody Mass", &Mass);

    m_UIHeight += (UINT)ImGui::GetItemRectSize().y;

    ImGui::Text("Jump Speed");
    ImGui::SameLine(100);
    ImGui::DragFloat("##Rigidbody JumpSpeed", &JumpSpeed);

    m_UIHeight += (UINT)ImGui::GetItemRectSize().y;

    ImGui::Text("Gravity Speed", &MaxGravitySpeed);
    ImGui::SameLine(100);
    ImGui::DragFloat("##Rigidbody MaxGravitySpeed", &MaxGravitySpeed);
    m_UIHeight += (UINT)ImGui::GetItemRectSize().y;
    m_UIHeight += (UINT)ImGui::GetItemRectSize().y;

    ImGui::Text("Ground Check");
    ImGui::SameLine(100);
    if (ImGui::Checkbox("##Rigidbody GroundCheck", &bGround))
    {
        pRB->SetGround(bGround);
    }
    m_UIHeight += (UINT)ImGui::GetItemRectSize().y;
    m_UIHeight += (UINT)ImGui::GetItemRectSize().y;
    
    ImGui::Text("Use Gravity");
    ImGui::SameLine(100);
    if (ImGui::Checkbox("##Rigidbody UseGravity?", &bUseGravity))
    {
        pRB->UseGravity(bUseGravity);
    }

    m_UIHeight += (UINT)ImGui::GetItemRectSize().y;
    m_UIHeight += (UINT)ImGui::GetItemRectSize().y;

    pRB->SetMass(Mass);
    pRB->SetJumpSpeed(JumpSpeed);
    pRB->SetMaxGravitySpeed(MaxGravitySpeed);

    SetChildSize(ImVec2(0.f, (float)m_UIHeight));
}