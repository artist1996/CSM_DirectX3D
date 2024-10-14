#include "pch.h"
#include "CHyungteoEatState.h"
#include <Engine/CLevelMgr.h>
#include <Scripts/CCameraMoveScript.h>

CHyungteoEatState::CHyungteoEatState()
	: CState(STATE_TYPE::HYUNGTEOEATSTATE)
	, m_Time(0.f)
	, m_CamMove(true)
{
}

CHyungteoEatState::~CHyungteoEatState()
{
}

void CHyungteoEatState::Enter()
{
	// Hyungteo Animation Num (0 : Eat, 1 : Look, 2 : Idle, 3 : Trace, 4 : Punch, 5 : Sting, 5 : Down)
	GetOwner()->Animator2D()->Play(0, 7.f, true);
}

void CHyungteoEatState::FinalTick()
{
	m_Time += DT;

	if (1.5f < m_Time && m_CamMove)
	{
		CGameObject* pCam = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera");
		pCam->Camera()->SetZoomIn(true);
		m_CamMove = false;
	}
	
	if (3.f < m_Time)
	{
		GetFSM()->ChangeState(L"Look");
	}
}

void CHyungteoEatState::Exit()
{
	m_Time = 0.f;
}