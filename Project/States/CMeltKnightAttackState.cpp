#include "pch.h"
#include "CMeltKnightAttackState.h"

CMeltKnightAttackState::CMeltKnightAttackState()
	: CState(STATE_TYPE::MELTKNIGHTATTACKSTATE)
	, m_AttackPref(nullptr)
	, m_Spawn(false)
{
}

CMeltKnightAttackState::~CMeltKnightAttackState()
{
}

void CMeltKnightAttackState::Enter()
{
	// Melt Knight Animation Number (0 : IDLE, 1 : Move, 2 : Attack, 3 : Dead)
	GetFSM()->GetOwner()->Animator2D()->Play(2, 7.f, true);
	m_AttackPref = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\meltknightattack.pref");
}

void CMeltKnightAttackState::FinalTick()
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
		GetFSM()->ChangeState(L"StandBy");
}

void CMeltKnightAttackState::Exit()
{
	m_Spawn = false;
}