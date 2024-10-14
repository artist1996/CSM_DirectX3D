#include "pch.h"
#include "CHyungteoDownHitState.h"

CHyungteoDownHitState::CHyungteoDownHitState()
	: CState(STATE_TYPE::HYUNGTEODOWNHITSTATE)
{
}

CHyungteoDownHitState::~CHyungteoDownHitState()
{
}

void CHyungteoDownHitState::Enter()
{
	GetOwner()->Animator2D()->Play(8, 10.f, false);
}

void CHyungteoDownHitState::FinalTick()
{
	CGameObject* pTarget = static_cast<CGameObject*>(GetFSM()->GetBlackboardData(L"Target"));

	if (L"Animation\\player\\punishershoot\\punishershoot.anim" != pTarget->GetChildren()[0]->Animator2D()->GetCurAnimation()->GetKey())
	{
		GetFSM()->ChangeState(L"WakeUp");
	}

	if (!GetOwner()->Rigidbody()->IsGround())
	{
		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		if (82.f > vPos.y)
		{
			GetOwner()->Rigidbody()->SetGround(true);
			GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 82.f, 0.f));
		}
	}
}

void CHyungteoDownHitState::Exit()
{
}