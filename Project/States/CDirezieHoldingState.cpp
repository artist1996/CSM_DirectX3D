#include "pch.h"
#include "CDirezieHoldingState.h"
#include <Engine/CLevelMgr.h>

CDirezieHoldingState::CDirezieHoldingState()
	: CState(STATE_TYPE::DIREZIEHOLDINGSTATE)
{
}

CDirezieHoldingState::~CDirezieHoldingState()
{
}

void CDirezieHoldingState::Enter()
{
	GetOwner()->Animator2D()->Play(8, 2.f, false);
	CLevelMgr::GetInst()->FindObjectByName(L"direziewind")->SetActive(false);
}

void CDirezieHoldingState::FinalTick()
{
	CGameObject* pTarget = static_cast<CGameObject*>(GetFSM()->GetBlackboardData(L"Target"));
	if (L"Animation\\player\\deathcrisis\\deathcrisis.anim" != pTarget->GetChildren()[0]->Animator2D()->GetCurAnimation()->GetKey())
	{
		GetFSM()->ChangeState(L"Idle");
		CLevelMgr::GetInst()->FindObjectByName(L"direziewind")->SetActive(true);
		return;
	}
}

void CDirezieHoldingState::Exit()
{
}