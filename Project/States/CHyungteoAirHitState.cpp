#include "pch.h"
#include "CHyungteoAirHitState.h"

CHyungteoAirHitState::CHyungteoAirHitState()
	: CState(STATE_TYPE::HYUNGTEOAIRHITSTATE)
{
}

CHyungteoAirHitState::~CHyungteoAirHitState()
{
}

void CHyungteoAirHitState::Enter()
{
	// Hyungteo Animation Num (0 : Eat, 1 : Look, 2 : Idle, 3 : Trace, 4 : Punch, 5 : Sting, 6 : Hit, 7 : Fly, 8 : Dead, 9 : WakeUp)

	GetOwner()->Animator2D()->Reset();
	GetOwner()->Animator2D()->Play(8, 10.f, false);
	GetOwner()->Rigidbody()->SetGround(false);
}

void CHyungteoAirHitState::FinalTick()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	INFO& info = GetOwner()->GetInfo();

	float GravitySpeed = GetOwner()->Rigidbody()->GetMaxGravitySpeed();
	GravitySpeed += 10.f;
	GetOwner()->Rigidbody()->SetMaxGravitySpeed(GravitySpeed);
	
	if (82.f > vPos.y)
	{
		GetOwner()->Rigidbody()->SetGround(true);
		GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 82.f, 0.f));
		GetFSM()->ChangeState(L"WakeUp");
	}
}

void CHyungteoAirHitState::Exit()
{
	CRigidbody* pRB = GetOwner()->Rigidbody();
	pRB->SetJumpSpeed(500.f);
	pRB->SetMaxGravitySpeed(500.f);
}