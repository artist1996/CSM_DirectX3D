#include "pch.h"
#include "CHyungteoStiffnessState.h"

CHyungteoStiffnessState::CHyungteoStiffnessState()
	: CState(STATE_TYPE::HYUNGTEOSTIFFNESSSTATE)
{
}

CHyungteoStiffnessState::~CHyungteoStiffnessState()
{
}

void CHyungteoStiffnessState::Enter()
{
	// Hyungteo Animation Num (0 : Eat, 1 : Look, 2 : Idle, 3 : Trace, 4 : Punch, 5 : Sting, 6 : Hit, 7 : Fly, 8 : WakeUp, 9 : Down)
	//GetOwner()->Animator2D()->Reset();
	GetOwner()->Animator2D()->Play(6, 15.f, false);
}

void CHyungteoStiffnessState::FinalTick()
{
	Vec3 vPos = GetOwner()->GetParent()->Transform()->GetRelativePos();

	OBJ_DIR Dir = GetOwner()->GetParent()->GetDir();

	if (OBJ_DIR::DIR_RIGHT == Dir)
	{
		vPos += Vec3(-1.f, 0.f, 0.f) * 10.f * DT;
	}

	else
	{
		vPos += Vec3(1.f, 0.f, 0.f) * 10.f * DT;
	}

	if (GetOwner()->Animator2D()->IsFinish())
		GetFSM()->ChangeState(L"Idle");

	GetOwner()->GetParent()->Transform()->SetRelativePos(vPos);
}

void CHyungteoStiffnessState::Exit()
{
}