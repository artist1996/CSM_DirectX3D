#include "pch.h"
#include "CJurisHideState.h"

CJurisHideState::CJurisHideState()
	: CState(STATE_TYPE::JURISHIDESTATE)
{
}

CJurisHideState::~CJurisHideState()
{
}

void CJurisHideState::Enter()
{
	// Juris Animation Num (0 : Idle, 1 : Move, 2 : Hide, 3 : Attack, 4 : Hit, 5 : Dead)
	GetOwner()->Animator2D()->Play(2, 15.f, true);
}

void CJurisHideState::FinalTick()
{
	Vec3 vPos = GetOwner()->GetParent()->Transform()->GetRelativePos();
	OBJ_DIR Dir = GetOwner()->GetParent()->GetDir();


	if (OBJ_DIR::DIR_LEFT == Dir)
		vPos += Vec3(1.f, 0.f, 0.f) * 75.f * DT;
	else if (OBJ_DIR::DIR_RIGHT == Dir)
		vPos += Vec3(-1.f, 0.f, 0.f) * 75.f * DT;

	if (GetOwner()->Animator2D()->IsFinish())
		GetFSM()->ChangeState(L"BackAttack");

	GetOwner()->GetParent()->Transform()->SetRelativePos(vPos);
}

void CJurisHideState::Exit()
{
}