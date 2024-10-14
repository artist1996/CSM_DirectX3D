#include "pch.h"
#include "CHyungteoFallState.h"

CHyungteoFallState::CHyungteoFallState()
	: CState(STATE_TYPE::HYUNGTEOFALLSTATE)
{
}

CHyungteoFallState::~CHyungteoFallState()
{
}

void CHyungteoFallState::Enter()
{

}

void CHyungteoFallState::FinalTick()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	if (82.f < vPos.y)
	{
		GetOwner()->Rigidbody()->SetGround(true);
		GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 82.f, 0.f));
		GetFSM()->ChangeState(L"WakeUp");
	}
}

void CHyungteoFallState::Exit()
{
}