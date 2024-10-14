#include "pch.h"
#include "CHyungteoDeadState.h"

CHyungteoDeadState::CHyungteoDeadState()
	: CState(STATE_TYPE::HYUNGTEODEADSTATE)
{
}

CHyungteoDeadState::~CHyungteoDeadState()
{
}

void CHyungteoDeadState::Enter()
{
	// Hyungteo Animation Num (0 : Eat, 1 : Look, 2 : Idle, 3 : Trace, 4 : Punch, 5 : Sting, 6 : Hit, 7 : Fly, 8 : Dead, 9 : WakeUp)
	GetOwner()->Animator2D()->Play(8, 10.f, false);
}

void CHyungteoDeadState::FinalTick()
{
	if (GetOwner()->Animator2D()->IsFinish())
		DeleteObject(GetOwner()->GetParent());
}

void CHyungteoDeadState::Exit()
{
}