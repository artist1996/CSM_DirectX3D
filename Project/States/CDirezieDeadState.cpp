#include "pch.h"
#include "CDirezieDeadState.h"

#include <Engine/CLevelMgr.h>

CDirezieDeadState::CDirezieDeadState()
	: CState(STATE_TYPE::DIREZIEDEADSTATE)
{
}

CDirezieDeadState::~CDirezieDeadState()
{
}

void CDirezieDeadState::Enter()
{
	GetOwner()->Animator2D()->Play(10, 4.f, false);
}

void CDirezieDeadState::FinalTick()
{
	if (GetOwner()->Animator2D()->IsFinish())
	{
		DeleteObject(GetOwner()->GetParent());
		DeleteObject(CLevelMgr::GetInst()->FindObjectByName(L"direziewind"));
	}
}

void CDirezieDeadState::Exit()
{
}