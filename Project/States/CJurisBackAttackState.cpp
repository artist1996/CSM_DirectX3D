#include "pch.h"
#include "CJurisBackAttackState.h"

CJurisBackAttackState::CJurisBackAttackState()
	: CState(STATE_TYPE::JURISBACKATTACKSTATE)
	, m_AttackPref(nullptr)
	, m_Init(true)
	, m_Spawn(false)
{
}

CJurisBackAttackState::~CJurisBackAttackState()
{
}

void CJurisBackAttackState::Enter()
{
	if (nullptr == m_AttackPref)
		m_AttackPref = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\jurisattack.pref");
}

void CJurisBackAttackState::FinalTick()
{
	CGameObject* pTarget = static_cast<CGameObject*>(GetFSM()->GetBlackboardData(L"Target"));

	Vec3 vPos = GetOwner()->GetParent()->Transform()->GetRelativePos();
	Vec3 vTargetPos = pTarget->Collider2D()->GetWorldPos();
	Vec3 vTargetScale = pTarget->Collider2D()->GetScale();

	OBJ_DIR Dir = pTarget->GetDir();

	if (m_Init)
	{
		if (OBJ_DIR::DIR_LEFT == Dir)
		{
			GetOwner()->GetParent()->SetDir(OBJ_DIR::DIR_LEFT);
			GetOwner()->GetParent()->Transform()->SetRelativePos(Vec3(vTargetPos.x + 125.f, vTargetPos.y + 75.f, vTargetPos.z));
			GetOwner()->GetParent()->Transform()->SetRelativeRotation(Vec3(0.f, XM_PI, 0.f));
		}
		else if (OBJ_DIR::DIR_RIGHT == Dir)
		{
			GetOwner()->GetParent()->SetDir(OBJ_DIR::DIR_RIGHT);
			GetOwner()->GetParent()->Transform()->SetRelativePos(Vec3(vTargetPos.x - 125.f, vTargetPos.y + 75.f, vTargetPos.z));
			GetOwner()->GetParent()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		}

		// Juris Animation Num (0 : Idle, 1 : Move, 2 : Hide, 3 : Attack, 4 : Hit, 5 : Dead)
		GetOwner()->Animator2D()->Play(3, 10.f, false);
		m_Init = false;
	}

	if (3 == GetOwner()->Animator2D()->GetCurFrameIndex() && !m_Spawn)
	{
		CGameObject* pObj = m_AttackPref->Instantiate();

		Vec3 vPos = GetOwner()->GetParent()->Collider2D()->GetWorldPos();
		Vec3 vScale = GetOwner()->GetParent()->Collider2D()->GetScale();

		Vec3 vFinalPos = vPos - vScale * 0.5f;

		if (OBJ_DIR::DIR_LEFT == GetOwner()->GetParent()->GetDir())
			pObj->Transform()->SetRelativePos(Vec3(vFinalPos.x - 75.f, vFinalPos.y, vFinalPos.z));
		else
			pObj->Transform()->SetRelativePos(Vec3(vFinalPos.x + 75.f, vFinalPos.y, vFinalPos.z));

		CreateObject(pObj, 8);
		m_Spawn = true;
	}

	if (GetOwner()->Animator2D()->IsFinish())
		GetFSM()->ChangeState(L"Idle");
}

void CJurisBackAttackState::Exit()
{
	m_Init = true;
	m_Spawn = false;
}