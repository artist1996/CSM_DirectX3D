#include "pch.h"
#include "CHyungteoTraceState.h"

#include <Engine/CRandomMgr.h>

CHyungteoTraceState::CHyungteoTraceState()
	: CState(STATE_TYPE::HYUNGTEOTRACESTATE)
{
}

CHyungteoTraceState::~CHyungteoTraceState()
{
}

void CHyungteoTraceState::Enter()
{
	// Hyungteo Animation Num (0 : Eat, 1 : Look, 2 : Idle, 3 : Trace, 4 : Punch, 5 : Sting, 5 : Down)
	//GetOwner()->Collider2D()->SetOffset(Vec3(0.f, -0.15f, 0.f));
	//GetOwner()->Collider2D()->SetScale(Vec3(0.4f, 0.2f, 0.f));
	GetOwner()->Animator2D()->Play(3, 7.f, true);
}

void CHyungteoTraceState::FinalTick()
{
	CGameObject* pTarget = static_cast<CGameObject*>(GetFSM()->GetBlackboardData(L"Target"));

	Vec3 vPos = GetOwner()->GetParent()->Transform()->GetRelativePos();

	Vec3 vTargetColPos = pTarget->Collider2D()->GetWorldPos();
	Vec3 vTargetColScale = pTarget->Collider2D()->GetScale();

	Vec3 vTargetPos = vTargetColPos - vTargetColScale * 0.5f;
	bool* bMoveable = GetOwner()->GetParent()->GetMoveable();

	Vec3 vDist = vTargetPos - vPos;

	vDist.Normalize();

	// 왼쪽/오른쪽, 위쪽/아래쪽이 모두 막힌 경우 우선 처리
	if (!bMoveable[(UINT)PLATFORM_TYPE::LEFT] && !bMoveable[(UINT)PLATFORM_TYPE::RIGHT])
	{
		vDist.x = 0.f;  // 양쪽 모두 이동 불가
	}
	else if (!bMoveable[(UINT)PLATFORM_TYPE::LEFT])
	{
		vDist.x = 1.f;  // 왼쪽만 이동 불가
	}
	else if (!bMoveable[(UINT)PLATFORM_TYPE::RIGHT])
	{
		vDist.x = -1.f;  // 오른쪽만 이동 불가
	}

	if (!bMoveable[(UINT)PLATFORM_TYPE::UP] && !bMoveable[(UINT)PLATFORM_TYPE::BOTTOM])
	{
		vDist.y = 0.f;  // 위쪽과 아래쪽 모두 이동 불가
	}
	else if (!bMoveable[(UINT)PLATFORM_TYPE::UP])
	{
		vDist.y = -1.f;  // 위쪽만 이동 불가
	}
	else if (!bMoveable[(UINT)PLATFORM_TYPE::BOTTOM])
	{
		vDist.y = 1.f;  // 아래쪽만 이동 불가
	}

	// 위쪽과 오른쪽이 동시에 막힌 경우 처리
	if (!bMoveable[(UINT)PLATFORM_TYPE::UP] && !bMoveable[(UINT)PLATFORM_TYPE::RIGHT])
	{
		vDist.x = 0.f;  // 오른쪽을 막고
		vDist.y = 1.f;   // 위쪽을 막으면 아래로 이동
	}

	// 위쪽과 왼쪽이 동시에 막힌 경우 처리
	else if (!bMoveable[(UINT)PLATFORM_TYPE::UP] && !bMoveable[(UINT)PLATFORM_TYPE::LEFT])
	{
		vDist.x = 0.f;   // 왼쪽을 막고
		vDist.y = 1.f;   // 위쪽을 막으면 아래로 이동
	}

	// 아래쪽과 오른쪽이 동시에 막힌 경우 처리
	else if (!bMoveable[(UINT)PLATFORM_TYPE::BOTTOM] && !bMoveable[(UINT)PLATFORM_TYPE::RIGHT])
	{
		vDist.x = 0.f;  // 오른쪽을 막고
		vDist.y = -1.f;  // 아래쪽을 막으면 위로 이동
	}

	// 아래쪽과 왼쪽이 동시에 막힌 경우 처리
	else if (!bMoveable[(UINT)PLATFORM_TYPE::BOTTOM] && !bMoveable[(UINT)PLATFORM_TYPE::LEFT])
	{
		vDist.x = 0.f;   // 왼쪽을 막고
		vDist.y = -1.f;  // 아래쪽을 막으면 위로 이동
	}

	// 모든 방향이 막힌 경우
	if (!bMoveable[(UINT)PLATFORM_TYPE::LEFT] &&
		!bMoveable[(UINT)PLATFORM_TYPE::RIGHT] &&
		!bMoveable[(UINT)PLATFORM_TYPE::UP] &&
		!bMoveable[(UINT)PLATFORM_TYPE::BOTTOM])
	{
		vDist.x = 0.f;
		vDist.y = 0.f;  // 모든 방향이 이동 불가
	}

	vPos.x += vDist.x * 100.f * DT;
	vPos.y += vDist.y * 100.f * DT;
	vPos.z = vPos.y;

	float fDist = vTargetPos.x - vPos.x;
	float fYDist = fabs(vTargetPos.y - vPos.y);
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


	if (100.f > fabs(fDist)
		&& 30.f > fabs(fYDist))
	{
		int Rand = CRandomMgr::GetInst()->GetRandom(1);

		if (0 == Rand)
			GetFSM()->ChangeState(L"Sting");
		else if (1 == Rand)
			GetFSM()->ChangeState(L"Punch");	
	}

	GetOwner()->GetParent()->Transform()->SetRelativePos(Vec3(vPos.x, vPos.y, vPos.y));
}

void CHyungteoTraceState::Exit()
{
}