#include "pch.h"
#include "CDirezieAttack05State.h"

CDirezieAttack05State::CDirezieAttack05State()
	: CState(STATE_TYPE::DIREZIEATTACK05STATE)
{
}

CDirezieAttack05State::~CDirezieAttack05State()
{
}

void CDirezieAttack05State::Enter()
{
	CGameObject* pTarget = static_cast<CGameObject*>(GetFSM()->GetBlackboardData(L"Target"));

	Vec3 vTargetColPos = pTarget->Collider2D()->GetWorldPos();
	Vec3 vTargetColScale = pTarget->Collider2D()->GetScale();

	Vec3 vTargetPos = vTargetColPos - vTargetColScale * 0.5f;

	GetOwner()->GetParent()->Transform()->SetRelativePos(vTargetPos);

	// Direzie Animation Num(0. idle, 1. move, 2. attack1, 3. attack2, 4. attack3, 5. attack4, 6. hide, 7. attack5)
	GetOwner()->Animator2D()->Play(7, 15.f, true);
	INFO& info = GetOwner()->GetInfo();
	info.bSuperArmor = true;
}

void CDirezieAttack05State::FinalTick()
{
	if (GetOwner()->Animator2D()->IsFinish())
		GetFSM()->ChangeState(L"Idle");
}

void CDirezieAttack05State::Exit()
{
}