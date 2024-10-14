#include "pch.h"
#include "CHyungteoWakeUpState.h"

CHyungteoWakeUpState::CHyungteoWakeUpState()
	: CState(STATE_TYPE::HYUNGTEOWAKEUPSTATE)
{
}

CHyungteoWakeUpState::~CHyungteoWakeUpState()
{
}

void CHyungteoWakeUpState::Enter()
{
	// Hyungteo Animation Num (0 : Eat, 1 : Look, 2 : Idle, 3 : Trace, 4 : Punch, 5 : Sting, 6 : Hit, 7 : Fly, 8 : Dead, 9 : WakeUp)
	//GetOwner()->Animator2D()->Play(8, 2.f, false);
	GetOwner()->Animator2D()->Reset();
	GetOwner()->Animator2D()->Play(9, 7.f, false);
	GetOwner()->Rigidbody()->SetGround(true);
}

void CHyungteoWakeUpState::FinalTick()
{
	if (GetOwner()->Animator2D()->IsFinish())
		GetFSM()->ChangeState(L"Idle");
}

void CHyungteoWakeUpState::Exit()
{
}