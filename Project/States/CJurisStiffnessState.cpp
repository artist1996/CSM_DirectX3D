#include "pch.h"
#include "CJurisStiffnessState.h"

CJurisStiffnessState::CJurisStiffnessState()
	: CState(STATE_TYPE::JURISSTIFFNESSSTATE)
{
}

CJurisStiffnessState::~CJurisStiffnessState()
{
}

void CJurisStiffnessState::Enter()
{
	// Juris Animation Num (0 : Idle, 1 : Move, 2 : Hide, 3 : Attack, 4 : Hit, 5 : Dead)
	GetOwner()->Animator2D()->Play(4, 10.f, false);
}

void CJurisStiffnessState::FinalTick()
{
	Vec3 vPos = GetOwner()->GetParent()->Transform()->GetRelativePos();

	OBJ_DIR Dir = GetOwner()->GetParent()->GetDir();

	if (OBJ_DIR::DIR_RIGHT == Dir)
		vPos += Vec3(-1.f, 0.f, 0.f) * 10.f * DT;
	else
		vPos += Vec3(1.f, 0.f, 0.f) * 10.f * DT;

	if (GetOwner()->Animator2D()->IsFinish())
		GetFSM()->ChangeState(L"Idle");

	GetOwner()->GetParent()->Transform()->SetRelativePos(vPos);
}

void CJurisStiffnessState::Exit()
{
}