#include "pch.h"
#include "CDirezieHideState.h"

CDirezieHideState::CDirezieHideState()
	: CState(STATE_TYPE::DIREZIEHIDESTATE)
	, m_CirclePref(nullptr)
	, m_Circle(nullptr)
{
}

CDirezieHideState::~CDirezieHideState()
{
}

void CDirezieHideState::Enter()
{
	// Direzie Animation Num(0. idle, 1. move, 2. attack1, 3. attack2, 4. attack3, 5. attack4, 6. hide, 7. attack5)
	Vec3 vPos = GetOwner()->Transform()->GetWorldPos();
	m_CirclePref = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"prefab\\direziecircle.pref");
	m_Circle = m_CirclePref->Instantiate();
	m_Circle->Transform()->SetRelativePos(Vec3(vPos.x, vPos.y + 100.f, vPos.z));
	CreateObject(m_Circle, 0);
	m_Circle->Animator2D()->Play(0, 8.f, false);
	GetOwner()->Animator2D()->Play(6, 8.f, true);
	INFO& info = GetOwner()->GetInfo();
	info.bSuperArmor = true;
}

void CDirezieHideState::FinalTick()
{
	if (nullptr != m_Circle
		&& m_Circle->Animator2D()->IsFinish())
	{
		DeleteObject(m_Circle);
		m_Circle = nullptr;
	}

	if (GetOwner()->Animator2D()->IsFinish())
	{
		GetFSM()->ChangeState(L"Attack05");
	}
}

void CDirezieHideState::Exit()
{
}