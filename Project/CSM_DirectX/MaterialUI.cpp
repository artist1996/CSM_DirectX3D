#include "pch.h"
#include "MaterialUI.h"

#include "ListUI.h"
#include "TreeUI.h"
#include "CEditorMgr.h"
#include "ParamUI.h"

#include <Engine/CAssetMgr.h>

MaterialUI::MaterialUI()
	: AssetUI(ASSET_TYPE::MATERIAL)
{
}

MaterialUI::~MaterialUI()
{
}

void MaterialUI::Update()
{
	Title();


	// Material Name
	OutputAssetName();
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	// Shader
	Ptr<CGraphicShader> pShader = pMtrl->GetShader();
	
	string strShaderName;

	if (nullptr == pShader)
		strShaderName = "None";
	else
		strShaderName = string(pShader->GetKey().begin(), pShader->GetKey().end());

	ImGui::Text("Shader Name");
	ImGui::SameLine(100);
	ImGui::InputText("##GraphicShaderName", (char*)strShaderName.c_str(), strShaderName.length(), ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* PayLoad = ImGui::AcceptDragDropPayload("ContentTree");

		if (PayLoad)
		{
			TreeNode** ppNode = (TreeNode**)PayLoad->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::GRAPHIC_SHADER == pAsset->GetAssetType())
			{
				pMtrl->SetShader((CGraphicShader*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##ShaderBtn", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("List");
		pListUI->SetName("GraphicShader");
		vector<string> vecMeshNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::GRAPHIC_SHADER, vecMeshNames);
		pListUI->AddList(vecMeshNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&MaterialUI::SelectShader);
		pListUI->SetActive(true);
	}

	// Shader Parameter
	ParameterTitle();
	ShaderParameter();

	ImGui::Separator();
	if(!pMtrl->IsEngineAsset())
		Save();
}

void MaterialUI::ShaderParameter()
{
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	Ptr<CGraphicShader> pShader = pMtrl->GetShader();

	if (nullptr == pShader)
		return;

	// Shader 가 요구하는 Parameter 정보를 가져온다.

	const vector<tScalarParam>& vecScalarParam = pShader->GetScalarParam();
	const vector<tTexParam>& vecTexParam = pShader->GetTexParam();

	for (size_t i = 0; i < vecScalarParam.size(); ++i)
	{
		switch (vecScalarParam[i].ParamType)
		{
		case INT_0:
		case INT_1:
		case INT_2:
		case INT_3:
		{
			int Data = *((int*)pMtrl->GetScalarParam(vecScalarParam[i].ParamType));
			if (ParamUI::DragInt(&Data, 1, vecScalarParam[i].strDesc))
			{
				pMtrl->SetScalarParam(vecScalarParam[i].ParamType, Data);
			}
		}
			break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
		{
			float Data = *((float*)pMtrl->GetScalarParam(vecScalarParam[i].ParamType));
			if (ParamUI::DragFloat(&Data, 1.f, vecScalarParam[i].strDesc))
			{
				pMtrl->SetScalarParam(vecScalarParam[i].ParamType, Data);
			}
		}
			break;
		case VEC2_0:
		case VEC2_1:
		case VEC2_2:
		case VEC2_3:
		{
			Vec2 Data = *((Vec2*)pMtrl->GetScalarParam(vecScalarParam[i].ParamType));
			if (ParamUI::DragVec2(&Data, 1.f, vecScalarParam[i].strDesc))
			{
				pMtrl->SetScalarParam(vecScalarParam[i].ParamType, Data);
			}
		}
			break;
		case VEC4_0:
		case VEC4_1:
		case VEC4_2:
		case VEC4_3:
		{
			Vec4 Data = *((Vec4*)pMtrl->GetScalarParam(vecScalarParam[i].ParamType));
			if (ParamUI::DragVec4(&Data, 0.1f, vecScalarParam[i].strDesc))
			{
				pMtrl->SetScalarParam(vecScalarParam[i].ParamType, Data);
			}
		}
			break;
		case MAT_0:
		case MAT_1:
		case MAT_2:
		case MAT_3:
		{

		}
			break;
		}
	}

	for (size_t i = 0; i < vecTexParam.size(); ++i)
	{
		Ptr<CTexture> pTexture = pMtrl->GetTexParam(vecTexParam[i].ParamType);

		if (ParamUI::InputTexture(pTexture, vecTexParam[i].strDesc, this, (DELEGATE_1)&MaterialUI::ChangeTexture))
		{
			m_SelectTexParam = vecTexParam[i].ParamType;
			pMtrl->SetTexParam(m_SelectTexParam, pTexture);
		}
	}
}

void MaterialUI::SelectShader(DWORD_PTR _ListUI)
{
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	
	ListUI* pList = (ListUI*)_ListUI;
	string strName = pList->GetSelectName();

	if ("None" == strName)
	{
		pMtrl->SetShader(nullptr);
		return;
	}

	wstring strShaderName = wstring(strName.begin(), strName.end());

	Ptr<CGraphicShader> pShader = CAssetMgr::GetInst()->FindAsset<CGraphicShader>(strShaderName);

	assert(pShader.Get());

	pMtrl->SetShader(pShader);
}

void MaterialUI::ChangeTexture(DWORD_PTR _Param)
{
	// 텍스쳐 파라미터를 입력받을 재질
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();

	// 마지막으로 선택한 항목이 무엇인지 ListUI 를 통해서 알아냄
	ListUI* pListUI = (ListUI*)_Param;
	string strName = pListUI->GetSelectName();

	if ("None" == strName)
	{
		pMtrl->SetTexParam(m_SelectTexParam, nullptr);
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);

	assert(pMtrl.Get());

	pMtrl->SetTexParam(m_SelectTexParam, pTex);
}

void MaterialUI::ParameterTitle()
{
	ImGui::PushID(0);
	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.8f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f, 0.7f, 0.8f));

	ImGui::Button("Shader Parameter");

	ImGui::PopStyleColor(3);
	ImGui::PopID();
}

void MaterialUI::Save()
{
	if (ImGui::Button("SAVE", ImVec2(40.f,18.f)))
	{
		SaveMaterialToFile();
	}
}

void MaterialUI::SaveMaterialToFile()
{
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();

	if (!pMtrl->IsEngineAsset())
	{
		pMtrl->Save(pMtrl->GetRelativePath());
	}
}