#include "pch.h"
#include "CDirezieFlyState.h"

CDirezieFlyState::CDirezieFlyState()
	: CState(STATE_TYPE::DIREZIEFLYSTATE)
{
}

CDirezieFlyState::~CDirezieFlyState()
{
}


void CDirezieFlyState::Enter()
{
	GetOwner()->Rigidbody()->Jump();

	// Hyungteo Animation Num (0 : Eat, 1 : Look, 2 : Idle, 3 : Trace, 4 : Punch, 5 : Sting, 6 : Hit, 7 : Fly, 8 : WakeUp, 9 : Down)
	GetOwner()->Animator2D()->Reset();
	GetOwner()->Animator2D()->Play(8, 2.f, false);
}

void CDirezieFlyState::FinalTick()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	if (87.f > vPos.y)
	{
		GetOwner()->Rigidbody()->SetGround(true);
		GetOwner()->Transform()->SetRelativePos(Vec3(-25.f, 87.f, 0.f));
		GetFSM()->ChangeState(L"WakeUp");
	}
}

void CDirezieFlyState::Exit()
{
}