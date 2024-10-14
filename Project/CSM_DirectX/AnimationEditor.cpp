#include "pch.h"
#include "AnimationEditor.h"

#include <Engine/CPathMgr.h>
#include <Engine/CAssetMgr.h>

#include "CEditorMgr.h"
#include "AE_Preview.h"
#include "AE_Detail.h"
#include "AE_Create.h"
#include "AE_SpriteView.h"

AnimationEditor::AnimationEditor()
	: m_Preview(nullptr)
	, m_Detail(nullptr)
	, m_Create(nullptr)
	, m_SV(nullptr)
{
	UseMenuBar(true);
}

AnimationEditor::~AnimationEditor()
{
}

void AnimationEditor::Init()
{
	m_Preview = (AE_Preview*)CEditorMgr::GetInst()->FindEditorUI("AE_Preview");
	m_Detail = (AE_Detail*)CEditorMgr::GetInst()->FindEditorUI("AE_Detail");
	m_Create = (AE_Create*)CEditorMgr::GetInst()->FindEditorUI("AE_Create");
	m_SV = (AE_SpriteView*)CEditorMgr::GetInst()->FindEditorUI("AE_SpriteView");

	m_Preview->SetMove(true);
	m_Detail->SetMove(true);
	m_SV->SetMove(true);

	m_Preview->m_Owner = this;
	m_Detail->m_Owner = this;
	m_Create->m_Owner = this;
	m_SV->m_Owner = this;

	SetActive(false);
}

void AnimationEditor::Update()
{
	if (!IsActive())
		return;

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Window"))
		{
			bool AtlasView = m_Preview->IsActive();
			bool Detail = m_Detail->IsActive();
			bool SV = m_SV->IsActive();

			if (ImGui::MenuItem("Preview", nullptr, &AtlasView))
			{
				m_Preview->SetActive(AtlasView);
			}

			if (ImGui::MenuItem("Detail", nullptr, &Detail))
			{
				m_Detail->SetActive(Detail);
			}

			if (ImGui::MenuItem("SV", nullptr, &SV))
			{
				m_SV->SetActive(SV);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load"))
			{
				LoadAnimation();
			}

			if (ImGui::MenuItem("Save"))
			{
				SaveAnimation();
			}

			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu("Create"))
		{
			if (ImGui::MenuItem("Create Empty Animation"))
			{
				m_Create->SetActive(true);
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void AnimationEditor::SaveAnimation()
{
	wchar_t szSelect[256] = {};
	wchar_t szRelativePath[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szSelect;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szSelect);
	ofn.lpstrFilter = L"모든 파일\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;

	// 탐색창 초기 위치 지정
	wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"Animation\\";
	ofn.lpstrInitialDir = strInitPath.c_str();

	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_ALLOWMULTISELECT | OFN_EXPLORER;

	if (GetSaveFileName(&ofn))
	{
		if (nullptr == m_Detail->GetAnimation())
			return;
		else
		{
			for (size_t i = 0; i < m_Detail->GetAnimation()->GetMaxFrameCount(); ++i)
			{
				path ContentPath = CPathMgr::GetInst()->GetContentPath();
				path RelativePath = std::filesystem::relative(szSelect, ContentPath);
								
				RelativePath = RelativePath.remove_filename();
				Ptr<CSprite> pSprite = m_Detail->GetAnimation()->GetSprite(i);
				path strKey = pSprite->GetKey();

				strKey = strKey.stem();

				RelativePath += strKey.wstring() + L".sprite";

				pSprite->Save(RelativePath.wstring());
			}
			
			m_Detail->GetAnimation()->Save(szSelect);
		}
	}
}

int AnimationEditor::LoadAnimation()
{
	wchar_t szSelect[256] = {};
	wchar_t szRelativePath[256] = {};

	OPENFILENAME ofn = {};

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = nullptr;
	ofn.lpstrFile = szSelect;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szSelect);
	ofn.lpstrFilter = L"모든 파일\0";
	ofn.nFilterIndex = 0;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;

	// 탐색창 초기 위치 지정
	wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
	strInitPath += L"Animation\\";
	ofn.lpstrInitialDir = strInitPath.c_str();

	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn))
	{
		wstring strConentPath = CPathMgr::GetInst()->GetContentPath();
		path RelativePath = std::filesystem::relative(szSelect, strConentPath);
		Ptr<CAnimation> pAnimation = CAssetMgr::GetInst()->Load<CAnimation>(RelativePath, RelativePath.wstring());

		if (nullptr == pAnimation)
			return E_FAIL;
		 
		m_Detail->SetAnimation(pAnimation);
		m_Preview->SetAnimation(pAnimation);
	}

	return S_OK;
}

void AnimationEditor::Activate()
{
	m_Preview->SetActive(true);
	m_Detail->SetActive(true);
	m_SV->SetActive(true);
}

void AnimationEditor::Deactivate()
{
    m_Preview->SetActive(false);
	m_Detail->SetActive(false);
	m_SV->SetActive(false);
	m_Create->SetActive(false);
}
