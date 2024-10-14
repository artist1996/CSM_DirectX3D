#include "pch.h"
#include "CHyungteoIdleState.h"

CHyungteoIdleState::CHyungteoIdleState()
	: CState(STATE_TYPE::HYUNGTEOIDLESTATE)
	, m_Time(0.f)
{
}

CHyungteoIdleState::~CHyungteoIdleState()
{
}

void CHyungteoIdleState::Enter()
{
	// Hyungteo Animation Num (0 : Eat, 1 : Look, 2 : Idle, 3 : Trace, 4 : Punch, 5 : Sting, 5 : Down)
	//GetOwner()->Collider2D()->SetOffset(Vec3(0.f, -0.05f, 0.f));
	//GetOwner()->Collider2D()->SetScale(Vec3(0.4f, 0.4f, 0.f));
	GetOwner()->Animator2D()->Play(2, 5.f, true);
}

void CHyungteoIdleState::FinalTick()
{
	m_Time += DT;

	INFO& info = GetOwner()->GetInfo();

	if (0.f >= info.HP)
		GetFSM()->ChangeState(L"Dead");

	if (1.f < m_Time)
		GetFSM()->ChangeState(L"Trace");
}

void CHyungteoIdleState::Exit()
{
	m_Time = 0.f;
}