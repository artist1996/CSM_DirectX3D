#include "pch.h"
#include "CMeltKnightIdleState.h"

CMeltKnightIdleState::CMeltKnightIdleState()
	: CState(STATE_TYPE::MELTKNIGHTIDLESTATE)
{
}

CMeltKnightIdleState::~CMeltKnightIdleState()
{
}

void CMeltKnightIdleState::Enter()
{
	// Melt Knight Animation Number (0 : IDLE, 1 : Move, 2 : Attack, 3 : Dead)
	GetFSM()->GetOwner()->Animator2D()->Play(0, 7.f, true);
}

void CMeltKnightIdleState::FinalTick()
{
	// Logic
	CGameObject* pTarget = (CGameObject*)GetFSM()->GetBlackboardData(L"Target");

	Vec3 vPos = GetOwner()->GetParent()->Transform()->GetRelativePos();
	Vec3 vTargetPos = pTarget->Transform()->GetRelativePos();
	
	Vec3 vDist = vTargetPos - vPos;

	if (0.f < vDist.x)
		GetOwner()->GetParent()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	else
		GetOwner()->GetParent()->Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));

	if (500.f > fabs(vPos.x - vTargetPos.x))
	{
		GetFSM()->ChangeState(L"Trace");
	}
}

void CMeltKnightIdleState::Exit()
{
	// Exit Logic
}