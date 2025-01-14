#include "pch.h"
#include "Animator3DUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CAnimator3D.h>

Animator3DUI::Animator3DUI()
	: ComponentUI(COMPONENT_TYPE::ANIMATOR3D)
{
}

Animator3DUI::~Animator3DUI()
{
}

void Animator3DUI::Init()
{
}

void Animator3DUI::Update()
{
	Title();

	CGameObject* pTarget = GetTargetObject();

	if (nullptr == pTarget)
		return;

	CAnimator3D* pAnim = pTarget->Animator3D();

	const vector<tMTAnimClip>* pVecClip = pAnim->GetCurClip();
	int curclip = pAnim->GetCurClipIdx();
	int bonecount = pAnim->GetBoneCount();
	
	ImGui::Text("Bone Count");
	ImGui::SameLine(120);
	ImGui::DragInt("##Animator3DBoneCount", &bonecount, 0.f, 0, 0);

	ImGui::Text("Clip Index");
	ImGui::SameLine(120);
	ImGui::DragInt("##Animator3DCurrentClipIndex", &curclip, 0.f, 0, 0);

	vector<string> vecAnimName;
	vector<const char*> vecPreview;

	for (size_t i = 0; i < pVecClip->size(); ++i)
	{
		vecAnimName.emplace_back(string(pVecClip->at(i).strAnimName.begin(), pVecClip->at(i).strAnimName.end()));
	}

	const char* curanimname = vecAnimName[curclip].c_str();

	for (size_t i = 0; i < vecAnimName.size(); ++i)
	{
		vecPreview.push_back(vecAnimName[i].c_str());
	}

	ImGui::Text("Clips");
	ImGui::SameLine(120);

	if (ImGui::BeginCombo("##Animation Clips", curanimname))
	{
		for (size_t i = 0; i < vecPreview.size(); ++i)
		{
			bool isSelected = (curclip == i);
			if (ImGui::Selectable(vecPreview[i], isSelected))
			{
				curclip = i;
				pAnim->SetClipIndex(curclip);
			}

			if (isSelected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::Text("Start Frame");
	ImGui::SameLine(120);
	int startframe = pVecClip->at(curclip).iStartFrame;

	ImGui::DragInt("##Start Frame", &startframe, 0, 0, 0);

	ImGui::Text("End Frame");
	ImGui::SameLine(120);
	int endframe = pVecClip->at(curclip).iEndFrame;
	ImGui::DragInt("##End Frame", &endframe, 0, 0, 0);
	
	int curframeidx = pAnim->GetFrameIdx();
	int nextframeidx = pAnim->GetNextFrameIdx();

	ImGui::Text("Cur Frame");
	ImGui::SameLine(120);
	ImGui::DragInt("##CurFrameIndex", &curframeidx, 0, 0, 0);

	ImGui::Text("Next Frame");
	ImGui::SameLine(120);
	ImGui::DragInt("##NextFrameIndex", &nextframeidx, 0, 0, 0);

	float starttime = (float)pVecClip->at(curclip).dStartTime;
	ImGui::Text("Start Time");
	ImGui::SameLine(120);
	ImGui::DragFloat("##Start Time", &starttime, 0.f, 0.f, 0.f);

	float endtime = (float)pVecClip->at(curclip).dEndTime;
	ImGui::Text("End Time");
	ImGui::SameLine(120);
	ImGui::DragFloat("##End Time", &endtime, 0.f, 0.f, 0.f);	
}