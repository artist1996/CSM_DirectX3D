#include "pch.h"
#include "Log.h"

#include <Engine/CTimeMgr.h>
#include <Engine/CKeyMgr.h>

Log::Log()
	: m_FPSTime(0.f)
	, m_FPS(0)
	, m_PrevFPS(0)
{
	SetMove(false);
}

Log::~Log()
{
}

void Log::Init()
{
	m_FPS = CTimeMgr::GetInst()->GetFPS();
	m_PrevFPS = m_FPS;
}

void Log::Update()
{
	//ImGui::SetWindowFocus();
	float fDT = EngineDT;
	//UINT FPS = CTimeMgr::GetInst()->GetFPS();

	m_FPS = CTimeMgr::GetInst()->GetEditFPS();
	ImGui::Text("DT      : %f", fDT);
	ImGui::Text("FPS     : %d", m_FPS);

	Vec2 vMousePos = CKeyMgr::GetInst()->GetMousePos();
	ImGui::Text("Mouse X : %f", vMousePos.x);
	ImGui::Text("Mouse Y : %f", vMousePos.y);
}