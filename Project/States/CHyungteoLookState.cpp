#include "pch.h"
#include "CHyungteoLookState.h"

#include <Engine/CLevelMgr.h>
#include <Scripts/CCameraMoveScript.h>

CHyungteoLookState::CHyungteoLookState()
	: CState(STATE_TYPE::HYUNGTEOLOOKSTATE)
{
}

CHyungteoLookState::~CHyungteoLookState()
{
}

void CHyungteoLookState::Enter()
{
	// Hyungteo Animation Num (0 : Eat, 1 : Look, 2 : Idle, 3 : Trace, 4 : Punch, 5 : Sting, 5 : Down)
	GetOwner()->Animator2D()->Play(1, 1.f, true);
}

void CHyungteoLookState::FinalTick()
{
	if (GetOwner()->Animator2D()->IsFinish())
		GetFSM()->ChangeState(L"Idle");
}

void CHyungteoLookState::Exit()
{
	CGameObject* pCam = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera");

	pCam->SetTarget(CLevelMgr::GetInst()->FindObjectByName(L"Player"));

	const vector<CScript*>& vecScripts = pCam->GetScripts();

	pCam->Camera()->SetZoomOut(true);
}