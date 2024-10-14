#include "pch.h"
#include "CJurisAttackState.h"

#include <Engine/CRandomMgr.h>

CJurisAttackState::CJurisAttackState()
	: CState(STATE_TYPE::JURISATTACKSTATE)
	, m_AttackPref(nullptr)
	, m_Spawn(false)
{
}

CJurisAttackState::~CJurisAttackState()
{
}

void CJurisAttackState::Enter()
{
	if (nullptr == m_AttackPref)
		m_AttackPref = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\jurisattack.pref");

	// Juris Animation Num (0 : Idle, 1 : Move, 2 : Hide, 3 : Attack, 4 : Hit, 5 : Dead)
	GetOwner()->Animator2D()->Play(3, 10.f, true);
}

void CJurisAttackState::FinalTick()
{
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

void CJurisAttackState::Exit()
{
	m_Spawn = false;
}