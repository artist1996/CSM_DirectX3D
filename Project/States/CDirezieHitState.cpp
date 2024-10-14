#include "pch.h"
#include "CDirezieHitState.h"

CDirezieHitState::CDirezieHitState()
	: CState(STATE_TYPE::DIREZIEHITSTATE)
{
}

CDirezieHitState::~CDirezieHitState()
{
}

void CDirezieHitState::Enter()
{
	// Direzie Animation Num(0. idle, 1. move, 2. attack1, 3. attack2, 4. attack3, 5. attack4, 6. hide, 7. attack5, 8. groundhit)
	GetOwner()->Animator2D()->Reset();
	GetOwner()->Animator2D()->Play(8, 9.f, false);
	INFO& info = GetOwner()->GetInfo();
	info.bSuperArmor = false;
}

void CDirezieHitState::FinalTick()
{
	if (GetOwner()->Animator2D()->IsFinish())
		GetFSM()->ChangeState(L"Idle");
}

void CDirezieHitState::Exit()
{
}