#include "pch.h"
#include "ViewportUI.h"

#include <Engine/CRenderMgr.h>

ViewportUI::ViewportUI()
{
	SetMove(false);
	SetHorizontalScrollBar(false);
}

ViewportUI::~ViewportUI()
{
}

void ViewportUI::Init()
{
}

void ViewportUI::Update()
{
	Ptr<CTexture> Viewport = CRenderMgr::GetInst()->GetCopyTex();
	
	//ImGui::Image((void*)Viewport->GetSRV().Get(), ImVec2(1280.f, 768.f));
	ImGui::Image((void*)Viewport->GetSRV().Get(), ImVec2(1024.f, 614.f));
}