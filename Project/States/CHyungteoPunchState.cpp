#include "pch.h"
#include "CHyungteoPunchState.h"

CHyungteoPunchState::CHyungteoPunchState()
	: CState(STATE_TYPE::HYUNGTEOPUNCHSTATE)
	, m_PunchPref(nullptr)
{
}

CHyungteoPunchState::~CHyungteoPunchState()
{
}

void CHyungteoPunchState::Enter()
{
	if (nullptr == m_PunchPref)
		m_PunchPref = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\hyungteopunch.pref");

	// Hyungteo Animation Num (0 : Eat, 1 : Look, 2 : Idle, 3 : Trace, 4 : Punch, 5 : Sting, 5 : Down)
	GetOwner()->Animator2D()->Play(4, 9.f, false);

	// Create Punch
	CGameObject* pObject = m_PunchPref->Instantiate();
	Vec3 vPos = GetOwner()->Transform()->GetWorldPos();

	if (OBJ_DIR::DIR_LEFT == GetOwner()->GetParent()->GetDir())
	{
		pObject->Transform()->SetRelativePos(Vec3(vPos.x - 75.f, vPos.y, vPos.z));
	}

	else if (OBJ_DIR::DIR_RIGHT == GetOwner()->GetParent()->GetDir())
	{
		pObject->Transform()->SetRelativePos(Vec3(vPos.x + 75.f, vPos.y, vPos.z));
	}

	CreateObject(pObject, 7);
}

void CHyungteoPunchState::FinalTick()
{
	if (GetOwner()->Animator2D()->IsFinish())
		GetFSM()->ChangeState(L"Idle");
}

void CHyungteoPunchState::Exit()
{
}