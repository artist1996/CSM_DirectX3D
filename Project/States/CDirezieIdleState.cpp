#include "pch.h"
#include "CDirezieIdleState.h"

#include <Engine/CRandomMgr.h>

CDirezieIdleState::CDirezieIdleState()
	: CState(STATE_TYPE::DIREZIEIDLESTATE)
	, m_Time(0.f)
{
}

CDirezieIdleState::~CDirezieIdleState()
{
}

void CDirezieIdleState::Enter()
{
	GetOwner()->Animator2D()->Play(0, 7.f, true);
	INFO& info = GetOwner()->GetInfo();

	info.bSuperArmor = false;
}

void CDirezieIdleState::FinalTick()
{
	CGameObject* pTarget = static_cast<CGameObject*>(GetFSM()->GetBlackboardData(L"Target"));
	Vec3 vPos = GetOwner()->GetParent()->Transform()->GetRelativePos();
	Vec3 vTargetPos = pTarget->Transform()->GetRelativePos();
	INFO& info = GetOwner()->GetInfo();
	Vec3 vDist = vTargetPos - vPos;

	if (0.f >= info.HP)
		GetFSM()->ChangeState(L"Dead");

	if (0.f < vDist.x)
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
		int Rand = CRandomMgr::GetInst()->GetRandom(3);
		
		if (0 == Rand)
			GetFSM()->ChangeState(L"Trace");
		else if (1 == Rand)
			GetFSM()->ChangeState(L"Attack03");
		else if (2 == Rand)
			GetFSM()->ChangeState(L"Attack04");
		else if (3 == Rand)
			GetFSM()->ChangeState(L"Hide");
	}
}

void CDirezieIdleState::Exit()
{
	m_Time = 0.f;
}