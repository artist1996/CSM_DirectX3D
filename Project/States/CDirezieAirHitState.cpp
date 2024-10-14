#include "pch.h"
#include "CDirezieAirHitState.h"

CDirezieAirHitState::CDirezieAirHitState()
	: CState(STATE_TYPE::DIREZIEAIRHITSTATE)
{
}

CDirezieAirHitState::~CDirezieAirHitState()
{
}

void CDirezieAirHitState::Enter()
{
	GetOwner()->Animator2D()->Reset();
	GetOwner()->Animator2D()->Play(8, 2.f, false);
	GetOwner()->Rigidbody()->SetGround(false);
}

void CDirezieAirHitState::FinalTick()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	GetOwner()->Rigidbody()->SetMaxGravitySpeed(700.f);
	if (87.f > vPos.y)
	{
		GetOwner()->Rigidbody()->SetGround(true);
		GetOwner()->Transform()->SetRelativePos(Vec3(-25.f, 87.f, 0.f));
		GetFSM()->ChangeState(L"Idle");
	}
}

void CDirezieAirHitState::Exit()
{
}