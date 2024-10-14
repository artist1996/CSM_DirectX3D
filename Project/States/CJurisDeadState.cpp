#include "pch.h"
#include "CJurisDeadState.h"

CJurisDeadState::CJurisDeadState()
	: CState(STATE_TYPE::JURISDEADSTATE)
{
}

CJurisDeadState::~CJurisDeadState()
{
}

void CJurisDeadState::Enter()
{
	// Juris Animation Num (0 : Idle, 1 : Move, 2 : Hide, 3 : Attack, 4 : Hit, 5 : Dead)
	GetOwner()->Animator2D()->Play(2, 6.f, false);
}

void CJurisDeadState::FinalTick()
{
	if (GetOwner()->Animator2D()->IsFinish())
		DeleteObject(GetOwner()->GetParent());
}

void CJurisDeadState::Exit()
{
}