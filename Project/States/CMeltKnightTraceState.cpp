#include "pch.h"
#include "CMeltKnightTraceState.h"

CMeltKnightTraceState::CMeltKnightTraceState()
	: CState(STATE_TYPE::MELTKNIGHTTRACESTATE)
{
}

CMeltKnightTraceState::~CMeltKnightTraceState()
{
}

void CMeltKnightTraceState::Enter()
{
	// Melt Knight Animation Number (0 : IDLE, 1 : Move, 2 : Attack, 3 : Dead)
	GetFSM()->GetOwner()->Animator2D()->Play(1, 7.f, true);
}

void CMeltKnightTraceState::FinalTick()
{
	CGameObject* pTarget = static_cast<CGameObject*>(GetFSM()->GetBlackboardData(L"Target"));

	Vec3 vPos = GetOwner()->GetParent()->Transform()->GetRelativePos();
	Vec3 vScale = GetOwner()->GetParent()->Transform()->GetRelativeScale();
	Vec3 vTargetColPos = pTarget->Collider2D()->GetWorldPos();
	Vec3 vTargetColScale = pTarget->Collider2D()->GetScale();

	Vec3 vTargetPos = vTargetColPos - vTargetColScale * 0.5f;
	
	bool* bMoveable = GetOwner()->GetParent()->GetMoveable();

	Vec3 vDist = vTargetPos - vPos;

	vDist.Normalize();
	
	// 왼쪽/오른쪽, 위쪽/아래쪽이 모두 막힌 경우 우선 처리
	if (!bMoveable[(UINT)PLATFORM_TYPE::LEFT] && !bMoveable[(UINT)PLATFORM_TYPE::RIGHT])
	{
		vDist.x = 0.f;
	}
	else if (!bMoveable[(UINT)PLATFORM_TYPE::LEFT])
	{
		vDist.x = 1.f;
	}
	else if (!bMoveable[(UINT)PLATFORM_TYPE::RIGHT])
	{
		vDist.x = -1.f;
	}

	if (!bMoveable[(UINT)PLATFORM_TYPE::UP] && !bMoveable[(UINT)PLATFORM_TYPE::BOTTOM])
	{
		vDist.y = 0.f;
	}
	else if (!bMoveable[(UINT)PLATFORM_TYPE::UP])
	{
		vDist.y = -1.f;
	}
	else if (!bMoveable[(UINT)PLATFORM_TYPE::BOTTOM])
	{
		vDist.y = 1.f;
	}

	// 위쪽과 오른쪽이 동시에 막힌 경우 처리
	if (!bMoveable[(UINT)PLATFORM_TYPE::UP] && !bMoveable[(UINT)PLATFORM_TYPE::RIGHT])
	{
		vDist.x = -1.f;  // 오른쪽을 막고
		vDist.y = 1.f;   // 위쪽을 막으면 아래로 이동
	}

	// 위쪽과 왼쪽이 동시에 막힌 경우 처리
	else if (!bMoveable[(UINT)PLATFORM_TYPE::UP] && !bMoveable[(UINT)PLATFORM_TYPE::LEFT])
	{
		vDist.x = 1.f;   // 왼쪽을 막고
		vDist.y = 1.f;   // 위쪽을 막으면 아래로 이동
	}

	// 아래쪽과 오른쪽이 동시에 막힌 경우 처리
	else if (!bMoveable[(UINT)PLATFORM_TYPE::BOTTOM] && !bMoveable[(UINT)PLATFORM_TYPE::RIGHT])
	{
		vDist.x = -1.f;  // 오른쪽을 막고
		vDist.y = -1.f;  // 아래쪽을 막으면 위로 이동
	}

	// 아래쪽과 왼쪽이 동시에 막힌 경우 처리
	else if (!bMoveable[(UINT)PLATFORM_TYPE::BOTTOM] && !bMoveable[(UINT)PLATFORM_TYPE::LEFT])
	{
		vDist.x = 1.f;   // 왼쪽을 막고
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

	Vec3 vColScale = GetOwner()->GetParent()->Collider2D()->GetScale();
	Vec3 vColPos = GetOwner()->GetParent()->Collider2D()->GetWorldPos();

	float Height = vColPos.y - vColScale.y * 0.5f;

	vPos.x += vDist.x * 50.f * DT;
	vPos.y += vDist.y * 50.f * DT;
	vPos.z = vPos.y;


	float fDist = vTargetPos.x - vPos.x;
	float fYDist = fabs(vTargetPos.y - vPos.y);
	if (0.f < fDist)
		GetOwner()->GetParent()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	else
		GetOwner()->GetParent()->Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));


	if (100.f > fabs(fDist) 
		&& 50.f > fabs(fYDist))
	{
		GetFSM()->ChangeState(L"Attack");
	}

	GetOwner()->GetParent()->Transform()->SetRelativePos(vPos);
	//GetOwner()->Transform()->SetZAxis(vPos.z);
}

void CMeltKnightTraceState::Exit()
{
}