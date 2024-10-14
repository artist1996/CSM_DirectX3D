#include "pch.h"
#include "CMeltKnightStandByState.h"

CMeltKnightStandByState::CMeltKnightStandByState()
	: CState(STATE_TYPE::MELTKNIGHTSTANDBYSTATE)
	, m_Time(0.f)
{
}

CMeltKnightStandByState::~CMeltKnightStandByState()
{
}

void CMeltKnightStandByState::Enter()
{
	// Melt Knight Animation Number (0 : IDLE, 1 : Move, 2 : Attack, 3 : Dead)
	GetFSM()->GetOwner()->Animator2D()->Play(0, 7.f, true);
}

void CMeltKnightStandByState::FinalTick()
{
	m_Time += DT;

	if (2.f < m_Time)
		GetFSM()->ChangeState(L"Idle");
}

void CMeltKnightStandByState::Exit()
{
	m_Time = 0.f;
}