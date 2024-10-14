#include "pch.h"
#include "CHyungteoRecoilState.h"

CHyungteoRecoilState::CHyungteoRecoilState()
	: CState(STATE_TYPE::HYUNGTEORECOILSTATE)
{
}

CHyungteoRecoilState::~CHyungteoRecoilState()
{
}

void CHyungteoRecoilState::Enter()
{
	// Hyungteo Animation Num (0 : Eat, 1 : Look, 2 : Idle, 3 : Trace, 4 : Punch, 5 : Sting, 6 : Hit, 7 : Fly, 8 : Dead, 9 : WakeUp)
	GetOwner()->Animator2D()->Play(8, 10.f, false);
}

void CHyungteoRecoilState::FinalTick()
{
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();

	if (82.f > vPos.y)
	{
		GetOwner()->Rigidbody()->SetGround(true);
		GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 82.f, 0.f));
		GetFSM()->ChangeState(L"WakeUp");
	}
}

void CHyungteoRecoilState::Exit()
{
}