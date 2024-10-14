#include "pch.h"
#include "CDirezieDownHitState.h"

CDirezieDownHitState::CDirezieDownHitState()
	: CState(STATE_TYPE::DIREZIEDOWNHITSTATE)
	, m_First(true)
	, m_Time(0.f)
{
}

CDirezieDownHitState::~CDirezieDownHitState()
{
}

void CDirezieDownHitState::Enter()
{
}

void CDirezieDownHitState::FinalTick()
{
	CGameObject* pTarget = static_cast<CGameObject*>(GetFSM()->GetBlackboardData(L"Target"));

	if (L"Animation\\player\\punishershoot\\punishershoot.anim" != pTarget->GetChildren()[0]->Animator2D()->GetCurAnimation()->GetKey())
	{
		m_Time += DT;

		if (0.3f < m_Time)
		{
			GetFSM()->ChangeState(L"Idle");
			m_Time = 0.f;
		}
	}

	if (!GetOwner()->Rigidbody()->IsGround())
	{
		Vec3 vPos = GetOwner()->Transform()->GetRelativePos();
		if (87.f > vPos.y)
		{
			GetOwner()->Rigidbody()->SetGround(true);
			GetOwner()->Transform()->SetRelativePos(Vec3(0.f, 82.f, 0.f));
		}
	}
}

void CDirezieDownHitState::Exit()
{
}