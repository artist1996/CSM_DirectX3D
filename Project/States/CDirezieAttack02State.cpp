#include "pch.h"
#include "CDirezieAttack02State.h"

CDirezieAttack02State::CDirezieAttack02State()
	: CState(STATE_TYPE::DIREZIEATTACK02STATE)
{
}

CDirezieAttack02State::~CDirezieAttack02State()
{
}

void CDirezieAttack02State::Enter()
{
	// Direzie Animation Num(0. idle, 1. move, 2. attack1, 3. attack2, 4. attack3, 5. attack4, 6. hide, 7. attack5)
	GetOwner()->Animator2D()->Play(3, 7.f, false);
	INFO& info = GetOwner()->GetInfo();
	info.bSuperArmor = true;
}

void CDirezieAttack02State::FinalTick()
{
	if (GetOwner()->Animator2D()->IsFinish())
		GetFSM()->ChangeState(L"Idle");
}

void CDirezieAttack02State::Exit()
{
}