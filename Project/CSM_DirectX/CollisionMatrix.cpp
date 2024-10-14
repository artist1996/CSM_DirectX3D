#include "pch.h"
#include "CollisionMatrix.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CCollisionMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

CollisionMatrix::CollisionMatrix()
	: m_MaxLength(0)
{
	SetActive(false);
}

CollisionMatrix::~CollisionMatrix()
{
}

void CollisionMatrix::Init()
{
}

void CollisionMatrix::Update()
{
	UINT* CollisionMatrix = CCollisionMgr::GetInst()->GetCollisionMatrix();
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	float posX = 19.375 * (MAX_LAYER / 4);
	float posY = ImGui::GetCursorPosY();

	ImGui::SetCursorPosX(posX);

	if (0 == m_MaxLength)
	{
		for (UINT i = 0; i < (MAX_LAYER / 2); ++i)
		{
			CLayer* pLayer = pLevel->GetLayer((MAX_LAYER / 2) - i - 1);
			string strName = string(pLayer->GetName().begin(), pLayer->GetName().end());

			if (strName.empty())
				strName = "None";

			if (m_MaxLength < strName.length())
				m_MaxLength = (UINT)strName.length();
		}
	}	

	for (UINT i = 0; i < (MAX_LAYER / 2); ++i)
	{
		CLayer* pLayer = pLevel->GetLayer((MAX_LAYER / 2) - i - 1);

		string strName = string(pLayer->GetName().begin(), pLayer->GetName().end());

		if (strName.empty())
			strName = "None";

		//ImGui::Text("%c", strName[0]);
		//ImGui::SameLine(posX);
		string space;
		if ((UINT)strName.length() < m_MaxLength)
		{
			UINT iDiff = m_MaxLength - (UINT)strName.length();
			space.append(iDiff, ' ');
		}

		strName = space + strName;

		for (UINT j = 0; j < strName.length(); ++j)
		{
			ImGui::SetCursorPosY(posY + j * 12.f);
			ImGui::Text("%c", strName[j]);
			ImGui::SameLine(posX);				
		}
	
		posX += 27.f;
		posY = ImGui::GetCursorPosY();
		
		ImGui::SameLine(posX);
	}

	ImGui::NewLine();
	
	for (UINT Row = 0; Row < MAX_LAYER / 2; ++Row)
	{
		CLayer* pLayer = pLevel->GetLayer(Row);
	
		string strLayerName = string(pLayer->GetName().begin(), pLayer->GetName().end());
	
		if (strLayerName.empty())
			ImGui::Text("None");
		else
			ImGui::Text(strLayerName.c_str());

		for(UINT Col = MAX_LAYER / 2 - Row - 1; Col < MAX_LAYER / 2; --Col)
		{		
			char szBuff[255] = {};
			bool bCheck = false;
			sprintf_s(szBuff, 255, "##LayerCheckBox%s%d", strLayerName.c_str(), Col);
		
			if (CollisionMatrix[Row] & (1 << Col + Row))
				bCheck = true;
			else
				bCheck = false;
	
			if (Col == MAX_LAYER / 2 - Row - 1)
				ImGui::SameLine(150);
			else
				ImGui::SameLine();
		
			if (ImGui::Checkbox(szBuff, &bCheck))
			{
				if (bCheck)
				{
					CCollisionMgr::GetInst()->CollisionCheck(Row, Col + Row);
					pLevel->CollisionCheck(Row, Col + Row);
				}
				else
				{
					CCollisionMgr::GetInst()->CollisionUnCheck(Row, Col + Row);
					pLevel->CollisionUnCheck(Row, Col + Row);
				}
			}
		}	
	}
}
