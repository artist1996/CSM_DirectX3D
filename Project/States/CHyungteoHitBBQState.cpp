#include "pch.h"
#include "CHyungteoHitBBQState.h"

CHyungteoHitBBQState::CHyungteoHitBBQState()
	: CState(STATE_TYPE::HYUNGTEOHITBBQSTATE)
	, m_InitHeight(82.f)
	, m_MaxHeight(400.f)
{
}

CHyungteoHitBBQState::~CHyungteoHitBBQState()
{
}

void CHyungteoHitBBQState::Enter()
{
	GetOwner()->Animator2D()->Play(8, 10.f, true);
}

void CHyungteoHitBBQState::FinalTick()
{
	CGameObject* pTarget = static_cast<CGameObject*>(GetFSM()->GetBlackboardData(L"Target"));
	Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vMovePos = GetOwner()->GetParent()->Transform()->GetRelativePos();

	Vec3 vTargetPos = pTarget->Transform()->GetRelativePos();

	Vec3 vDist = vTargetPos - vMovePos;
	Vec3 vNormal = vTargetPos - vMovePos;
	vNormal.Normalize();

	if (300.f > fabs(vDist.x))
	{
		if (OBJ_DIR::DIR_RIGHT == pTarget->GetDir())
			vMovePos.x += 1.f * 400.f * DT;
		else
			vMovePos.x -= 1.f * 400.f * DT;
	}

	if (m_MaxHeight <= vPos.y)
		GetOwner()->Rigidbody()->SetGround(true);

	if (L"Animation\\player\\bbqshoot\\bbqshoot.anim" != pTarget->GetChildren()[0]->Animator2D()->GetCurAnimation()->GetKey()
		&& L"Animation\\player\\bbqready\\bbqready.anim" != pTarget->GetChildren()[0]->Animator2D()->GetCurAnimation()->GetKey())
	{
		GetFSM()->ChangeState(L"AirHit");
		return;
	}

	GetOwner()->GetParent()->Transform()->SetRelativePos(vMovePos);
}

void CHyungteoHitBBQState::Exit()
{
}