#include "pch.h"
#include "CHyungteoFlyState.h"

CHyungteoFlyState::CHyungteoFlyState()
	: CState(STATE_TYPE::HYUNGTEOFLYSTATE)
	, m_InitJumpPos(0.f)
{
}

CHyungteoFlyState::~CHyungteoFlyState()
{
}

void CHyungteoFlyState::Enter()
{
	GetOwner()->Rigidbody()->Jump();
	//Vec3 vPos = GetOwner()->Transform()->GetWorldPos();
	//GetOwner()->Transform()->SetRelativePos(Vec3(vPos.x, vPos.y, m_InitJumpPos));

	// Hyungteo Animation Num (0 : Eat, 1 : Look, 2 : Idle, 3 : Trace, 4 : Punch, 5 : Sting, 6 : Hit, 7 : Fly, 8 : WakeUp, 9 : Down)
	GetOwner()->Animator2D()->Reset();
	GetOwner()->Animator2D()->Play(8, 2.f, false);
	//GetOwner()->Animator2D()->Play(2, 2.f, false);
}

void CHyungteoFlyState::FinalTick()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	bool IsFall = GetOwner()->Rigidbody()->IsFall();

	if (82.f > vPos.y)
	{
		GetOwner()->Rigidbody()->SetGround(true);
		GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 82.f, 0.f));
		GetFSM()->ChangeState(L"WakeUp");
	}
}

void CHyungteoFlyState::Exit()
{
}