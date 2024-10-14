#include "pch.h"
#include "SpriteEditor.h"

#include "CEditorMgr.h"
#include "SE_TextureVIew.h"
#include "SE_Info.h"
#include "SE_Create.h"

SpriteEditor::SpriteEditor()
	: m_TextureView(nullptr)
	, m_SpriteInfo(nullptr)
	, m_Create(nullptr)
{
	UseMenuBar(true);
}

SpriteEditor::~SpriteEditor()
{
}

void SpriteEditor::Init()
{
	m_TextureView = (SE_TextureView*)CEditorMgr::GetInst()->FindEditorUI("SE_TextureView");
	m_SpriteInfo = (SE_Info*)CEditorMgr::GetInst()->FindEditorUI("SE_Info");
	m_Create = (SE_Create*)CEditorMgr::GetInst()->FindEditorUI("SE_Create");

	m_TextureView->SetMove(false);
	m_SpriteInfo->SetMove(false);

	m_TextureView->m_Owner = this;
	m_SpriteInfo->m_Owner = this;
	m_Create->m_Owner = this;

	SetActive(false);
	Deactivate();
}

void SpriteEditor::Update()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Window"))
		{
			bool AtlasView = m_TextureView->IsActive();
			bool Detail = m_SpriteInfo->IsActive();

			if (ImGui::MenuItem("AtlasView", nullptr, &AtlasView))
			{
				m_TextureView->SetActive(AtlasView);
			}

			if (ImGui::MenuItem("Detail", nullptr, &Detail))
			{
				m_SpriteInfo->SetActive(Detail);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void SpriteEditor::Activate()
{
	m_TextureView->SetActive(true);
	m_SpriteInfo->SetActive(true);
}

void SpriteEditor::Deactivate()
{
	m_TextureView->SetActive(false);
	m_SpriteInfo->SetActive(false);
}
