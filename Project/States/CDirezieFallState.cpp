#include "pch.h"
#include "CDirezieFallState.h"

CDirezieFallState::CDirezieFallState()
	: CState(STATE_TYPE::DIREZIEFALLSTATE)
{
}

CDirezieFallState::~CDirezieFallState()
{
}

void CDirezieFallState::Enter()
{
	GetOwner()->Rigidbody()->SetMaxGravitySpeed(600.f);
	GetOwner()->Rigidbody()->SetGround(false);
	GetOwner()->Animator2D()->Play(8, 2.f, false);
}

void CDirezieFallState::FinalTick()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	if (87.f > vPos.y)
	{
		GetOwner()->Rigidbody()->SetGround(true);
		GetOwner()->Transform()->SetRelativePos(Vec3(-25.f, 87.f, 0.f));
		GetFSM()->ChangeState(L"WakeUp");
	}
}

void CDirezieFallState::Exit()
{
}