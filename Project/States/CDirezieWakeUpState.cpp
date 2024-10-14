#include "pch.h"
#include "CDirezieWakeUpState.h"

CDirezieWakeUpState::CDirezieWakeUpState()
	: CState(STATE_TYPE::DIREZIEWAKEUPSTATE)
{
}

CDirezieWakeUpState::~CDirezieWakeUpState()
{
}

void CDirezieWakeUpState::Enter()
{
	GetOwner()->Animator2D()->Play(9, 4.f, false);
}

void CDirezieWakeUpState::FinalTick()
{
	if (GetOwner()->Animator2D()->IsFinish())
		GetFSM()->ChangeState(L"Idle");
}

void CDirezieWakeUpState::Exit()
{
}