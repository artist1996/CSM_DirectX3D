#include "pch.h"
#include "ListUI.h"

ListUI::ListUI()
	: m_SelectedIdx(-1)
	, m_UI(nullptr)
	, m_MemFunc(nullptr)
{
}

ListUI::~ListUI()
{
}

void ListUI::Update()
{
	for (size_t i = 0; i < m_vecList.size(); ++i)
	{
		char szID[255] = {};
		sprintf_s(szID, 255, "%s##%d", m_vecList[i].c_str(), (int)i);

		UINT Flag = ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Leaf;

		if (i == m_SelectedIdx)
		{
			Flag |= ImGuiTreeNodeFlags_::ImGuiTreeNodeFlags_Selected;
		}

		if (ImGui::TreeNodeEx(szID, Flag))
		{
			ImGui::TreePop();
		}
		
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			m_SelectedIdx = (int)i;
		}

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			m_SelectedName = m_vecList[i];

			if (m_UI && m_MemFunc)
			{
				(m_UI->*m_MemFunc)((DWORD_PTR)this);
			}
			
			SetActive(false);
		}
	}
}

void ListUI::AddList(const vector<string>& _List)
{
	m_vecList.insert(m_vecList.end(), _List.begin(), _List.end());
}

void ListUI::Activate()
{
	m_vecList.insert(m_vecList.begin(), 1, "None");
}

void ListUI::Deactivate()
{
	m_vecList.clear();
	m_SelectedIdx = -1;
	m_UI = nullptr;
	m_MemFunc = nullptr;
}
