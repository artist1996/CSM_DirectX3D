#include "pch.h"
#include "CHyungteoHitState.h"

CHyungteoHitState::CHyungteoHitState()
	: CState(STATE_TYPE::HYUNGTEOHITSTATE)
{
}

CHyungteoHitState::~CHyungteoHitState()
{
}

void CHyungteoHitState::Enter()
{
	// Hyungteo Animation Num (0 : Eat, 1 : Look, 2 : Idle, 3 : Trace, 4 : Punch, 5 : Sting, 6 : Hit, 7 : Fly, 8 : WakeUp, 9 : Down)
	//GetOwner()->Animator2D()->Reset();
	GetOwner()->Animator2D()->Play(6, 15.f, false);
}

void CHyungteoHitState::FinalTick()
{
	if (GetOwner()->Animator2D()->IsFinish())
		GetFSM()->ChangeState(L"Idle");
}

void CHyungteoHitState::Exit()
{
}