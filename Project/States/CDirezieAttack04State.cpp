#include "pch.h"
#include "CDirezieAttack04State.h"

CDirezieAttack04State::CDirezieAttack04State()
	: CState(STATE_TYPE::DIREZIEATTACK04STATE)
	, m_Time(0.f)
{
}

CDirezieAttack04State::~CDirezieAttack04State()
{
}

void CDirezieAttack04State::Enter()
{
	if (nullptr == m_Pref)
		m_Pref = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\direzie.pref");

	// Direzie Animation Num(0. idle, 1. move, 2. attack1, 3. attack2, 4. attack3, 5. attack4, 6. hide, 7. attack5)
	GetOwner()->Animator2D()->Play(5, 7.f, true);
	INFO& info = GetOwner()->GetInfo();
	info.bSuperArmor = true;
}

void CDirezieAttack04State::FinalTick()
{
	m_Time += DT;

	CGameObject* pTarget = static_cast<CGameObject*>(GetFSM()->GetBlackboardData(L"Target"));
	
	Vec3 vPos = GetOwner()->GetParent()->Transform()->GetRelativePos();

	Vec3 vTargetColPos = pTarget->Collider2D()->GetWorldPos();
	Vec3 vTargetColScale = pTarget->Collider2D()->GetScale();

	Vec3 vTargetPos = vTargetColPos - vTargetColScale * 0.5f;
	bool* bMoveable = GetOwner()->GetParent()->GetMoveable();

	Vec3 vDist = vTargetPos - vPos;

	vDist.Normalize();

	if (3.f < m_Time)
	{
		GetFSM()->ChangeState(L"Idle");
	}

	vPos += vDist * 200.f * DT;

	GetOwner()->GetParent()->Transform()->SetRelativePos(vPos);
}

void CDirezieAttack04State::Exit()
{
	m_Time = 0.f;
}