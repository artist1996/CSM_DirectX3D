#include "pch.h"
#include "CMeltKnightDeadState.h"

CMeltKnightDeadState::CMeltKnightDeadState()
	: CState(STATE_TYPE::MELTKNIGHTDEADSTATE)
{
}

CMeltKnightDeadState::~CMeltKnightDeadState()
{
}

void CMeltKnightDeadState::Enter()
{
	// Melt Knight Animation Number (0 : IDLE, 1 : Move, 2 : Attack, 3 : Dead)
	GetOwner()->Animator2D()->Reset();
	GetOwner()->Animator2D()->Play(3, 7.f, true);
}

void CMeltKnightDeadState::FinalTick()
{
	if (GetOwner()->Animator2D()->IsFinish())
	{
		DeleteObject(GetOwner()->GetParent());
	}
}

void CMeltKnightDeadState::Exit()
{
}