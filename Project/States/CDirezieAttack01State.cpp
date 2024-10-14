#include "pch.h"
#include "CDirezieAttack01State.h"

CDirezieAttack01State::CDirezieAttack01State()
	: CState(STATE_TYPE::DIREZIEATTACK01STATE)
{
}

CDirezieAttack01State::~CDirezieAttack01State()
{
}

void CDirezieAttack01State::Enter()
{
	// Direzie Animation Num(0. idle, 1. move, 2. attack1, 3. attack2, 4. attack3, 5. attack4, 6. hide, 7. attack5)
	GetOwner()->Animator2D()->Play(2, 7.f, false);
	INFO& info = GetOwner()->GetInfo();
	info.bSuperArmor = true;
}

void CDirezieAttack01State::FinalTick()
{
	if (GetOwner()->Animator2D()->IsFinish())
		GetFSM()->ChangeState(L"Idle");
}

void CDirezieAttack01State::Exit()
{
}