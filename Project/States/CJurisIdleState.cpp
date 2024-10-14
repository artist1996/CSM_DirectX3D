#include "pch.h"
#include "CJurisIdleState.h"

#include <Engine/CRandomMgr.h>

CJurisIdleState::CJurisIdleState()
	: CState(STATE_TYPE::JURISIDLESTATE)
	, m_Time(0.f)
{
}

CJurisIdleState::~CJurisIdleState()
{
}

void CJurisIdleState::Enter()
{
	// Juris Animation Num (0 : Idle, 1 : Move, 2 : Hide, 3 : Attack, 4 : Hit, 5 : Dead)
	GetOwner()->Animator2D()->Reset();
	GetOwner()->Animator2D()->Play(0, 6.f, true);
}

void CJurisIdleState::FinalTick()
{
	CGameObject* pTarget = static_cast<CGameObject*>(GetFSM()->GetBlackboardData(L"Target"));
	
	Vec3 vPos = GetOwner()->GetParent()->Transform()->GetRelativePos();
	Vec3 vTargetPos = pTarget->Transform()->GetRelativePos();

	float fDist = vTargetPos.x - vPos.x;
	INFO& info = GetOwner()->GetInfo();

	if (0.f >= info.HP)
		GetFSM()->ChangeState(L"Dead");

	if (0.f < fDist)
	{
		GetOwner()->GetParent()->SetDir(OBJ_DIR::DIR_RIGHT);
		GetOwner()->GetParent()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	}
	else
	{
		GetOwner()->GetParent()->SetDir(OBJ_DIR::DIR_LEFT);
		GetOwner()->GetParent()->Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));
	}

	m_Time += DT;

	if (1.f < m_Time)
	{
		int Rand = CRandomMgr::GetInst()->GetRandom(1);

		if (0 == Rand)
			GetFSM()->ChangeState(L"Trace");
		else if (1 == Rand)
			GetFSM()->ChangeState(L"Hide");
	}
}

void CJurisIdleState::Exit()
{
	m_Time = 0.f;
}