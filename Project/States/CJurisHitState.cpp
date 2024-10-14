#include "pch.h"
#include "CJurisHitState.h"

CJurisHitState::CJurisHitState()
	: CState(STATE_TYPE::JURISHITSTATE)
{
}

CJurisHitState::~CJurisHitState()
{
}

void CJurisHitState::Enter()
{
	// Juris Animation Num (0 : Idle, 1 : Move, 2 : Hide, 3 : Attack, 4 : Hit, 5 : Dead)
	GetOwner()->Animator2D()->Play(4, 5.f, false);
}

void CJurisHitState::FinalTick()
{
	if (GetOwner()->Animator2D()->IsFinish())
		GetFSM()->ChangeState(L"Idle");
}

void CJurisHitState::Exit()
{
}