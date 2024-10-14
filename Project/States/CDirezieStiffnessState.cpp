#include "pch.h"
#include "CDirezieStiffnessState.h"

CDirezieStiffnessState::CDirezieStiffnessState()
	: CState(STATE_TYPE::DIREZIESTIFFNESSSTATE)
{
}

CDirezieStiffnessState::~CDirezieStiffnessState()
{
}

void CDirezieStiffnessState::Enter()
{
	GetOwner()->Animator2D()->Play(8, 9.f, false);
	INFO& info = GetOwner()->GetInfo();
	info.bSuperArmor = false;
}

void CDirezieStiffnessState::FinalTick()
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

void CDirezieStiffnessState::Exit()
{
}