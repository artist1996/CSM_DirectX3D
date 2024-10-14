#include "pch.h"
#include "CHyungteoBuffEaterState.h"

CHyungteoBuffEaterState::CHyungteoBuffEaterState()
	: CState(STATE_TYPE::HYUNGTEOBUFFEATERSTATE)
	, m_BuffEaterPref(nullptr)
{
}

CHyungteoBuffEaterState::~CHyungteoBuffEaterState()
{
}

void CHyungteoBuffEaterState::Enter()
{
	if(nullptr == m_BuffEaterPref)
		m_BuffEaterPref = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\hyungteopunch.pref");

	// Hyungteo Animation Num (0 : Eat, 1 : Look, 2 : Idle, 3 : Trace, 4 : Punch, 5 : Sting, 6 : Hit, 7 : Fly, 8 : WakeUp, 9 : Down)
	GetOwner()->Animator2D()->Play(2, 9.f, false);

	Vec3 vPos = GetOwner()->Transform()->GetWorldPos();

	CGameObject* pObject = m_BuffEaterPref->Instantiate();
	pObject->Transform()->SetRelativePos(vPos);
}

void CHyungteoBuffEaterState::FinalTick()
{
	if (GetOwner()->Animator2D()->IsFinish())
		GetFSM()->ChangeState(L"Idle");
}

void CHyungteoBuffEaterState::Exit()
{
}