#include "pch.h"
#include "TileMapEditor.h"

#include "CEditorMgr.h"

#include "TE_Detail.h"
#include "TE_TextureView.h"
#include "TE_Renderer.h"

TileMapEditor::TileMapEditor()
	: m_Detail(nullptr)
	, m_TextureView(nullptr)
	, m_Renderer(nullptr)
{
	UseMenuBar(true);
}

TileMapEditor::~TileMapEditor()
{
}

void TileMapEditor::SetTargetObject(CGameObject* _TargetObject)
{
	m_Detail->SetTargetObject(_TargetObject);
	m_TextureView->SetTargetObject(_TargetObject);
	m_Renderer->SetTargetObject(_TargetObject);
}

void TileMapEditor::Init()
{
	m_Detail = (TE_Detail*)CEditorMgr::GetInst()->FindEditorUI("TE_Detail");
	m_TextureView = (TE_TextureView*)CEditorMgr::GetInst()->FindEditorUI("TE_TextureView");
	m_Renderer = (TE_Renderer*)CEditorMgr::GetInst()->FindEditorUI("TE_Renderer");

	m_Detail->SetMove(false);
	m_TextureView->SetMove(false);
	m_Renderer->SetMove(false);

	m_Detail->m_Owner = this;
	m_TextureView->m_Owner = this;
	m_Renderer->m_Owner = this;

	SetActive(false);
}

void TileMapEditor::Update()
{
	if (!IsActive())
		return;

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Window"))
		{
			bool Detail = m_Detail->IsActive();
			bool TextureView = m_TextureView->IsActive();
			bool Renderer = m_Renderer->IsActive();
	
			if (ImGui::MenuItem("Detail", nullptr, &Detail))
			{
				m_Detail->SetActive(Detail);
			}
	
			if (ImGui::MenuItem("Texture View", nullptr, &TextureView))
			{
				m_TextureView->SetActive(Detail);
			}
	
			if (ImGui::MenuItem("TileMap Renderer", nullptr, &Renderer))
			{
				m_Renderer->SetActive(Renderer);
			}
	
			ImGui::EndMenu();
		}
	
		ImGui::EndMenuBar();
	}
}

void TileMapEditor::Activate()
{
	m_Detail->SetActive(true);
	m_TextureView->SetActive(true);
	m_Renderer->SetActive(true);
}

void TileMapEditor::Deactivate()
{
	m_Detail->SetActive(false);
	m_TextureView->SetActive(false);
	m_Renderer->SetActive(false);
}
