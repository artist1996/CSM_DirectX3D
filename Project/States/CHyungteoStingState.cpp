#include "pch.h"
#include "CHyungteoStingState.h"

CHyungteoStingState::CHyungteoStingState()
	: CState(STATE_TYPE::HYUNGTEOSTINGSTATE)
	, m_StingPref(nullptr)
{
}

CHyungteoStingState::~CHyungteoStingState()
{
}

void CHyungteoStingState::Enter()
{
	if (nullptr == m_StingPref)
		m_StingPref = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\hyungteosting.pref");

	// Hyungteo Animation Num (0 : Eat, 1 : Look, 2 : Idle, 3 : Trace, 4 : Punch, 5 : Sting, 6 : Hit, 7 : Fly, 8 : WakeUp, 9 : Down)
	GetOwner()->Animator2D()->Play(5, 9.f, false);

	// Create Punch
	CGameObject* pObject = m_StingPref->Instantiate();
	Vec3 vPos = GetOwner()->Transform()->GetWorldPos();

	if (OBJ_DIR::DIR_LEFT == GetOwner()->GetParent()->GetDir())
	{
		pObject->Transform()->SetRelativePos(Vec3(vPos.x - 125.f, vPos.y, vPos.z));
	}

	else if (OBJ_DIR::DIR_RIGHT == GetOwner()->GetParent()->GetDir())
	{
		pObject->Transform()->SetRelativePos(Vec3(vPos.x + 125.f, vPos.y, vPos.z));
	}

	CreateObject(pObject, 7);
}

void CHyungteoStingState::FinalTick()
{
	if (GetOwner()->Animator2D()->IsFinish())
		GetFSM()->ChangeState(L"Idle");
}

void CHyungteoStingState::Exit()
{
}