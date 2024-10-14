#include "pch.h"
#include "ParticleSystemUI.h"

#include <Engine/CGameObject.h>
#include <Engine/CParticleSystem.h>

#include "TreeUI.h"

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_bChange(false)
{
}

ParticleSystemUI::~ParticleSystemUI()
{
}

void ParticleSystemUI::Update()
{
	Title();

	CParticleSystem* pParticleSys = GetTargetObject()->ParticleSystem();

	Ptr<CTexture> ParticleTex = pParticleSys->GetParticleTex();
	int MaxParticleCount = pParticleSys->GetMaxParticleCount();
	tParticleModule& Module = pParticleSys->GetModuleInfo();

	// Texture Image
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

	ImGui::Image(ParticleTex->GetSRV().Get(), ImVec2(100.f, 100.f), uv_min, uv_max, tint_col, border_col);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::TEXTURE == pAsset->GetAssetType())
			{
				pParticleSys->SetParticleTexture((CTexture*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::Separator();
	// Spawn
	ImGui::Text("Spawn Module");
	ImGui::SameLine(130);

	bool SpawnModule = Module.Module[(UINT)PARTICLE_MODULE::SPAWN];

	if (ImGui::Checkbox("##Spawn Module", &SpawnModule))
	{
		Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = SpawnModule;
	}

	ImGui::Text("Spawn Rate");
	ImGui::SameLine(130);
	ImGui::DragInt("##SpawnRate", (int*)&Module.SpawnRate);

	ImGui::Text("Spawn Color");
	ImGui::SameLine(130);
	ImGui::ColorEdit4("##SpawnColor", Module.vSpawnColor);
	
	ImGui::Text("Min Scale");
	ImGui::SameLine(130);
	ImGui::DragFloat4("##SpawnMinScale", Module.vSpawnMinScale);

	ImGui::Text("Max Scale");
	ImGui::SameLine(130);
	ImGui::DragFloat4("##SpawnMaxScale", Module.vSpawnMaxScale);

	ImGui::Text("Min Life");
	ImGui::SameLine(130);
	ImGui::DragFloat("##SpawnMinLife", &Module.MinLife);

	ImGui::Text("Max Life");
	ImGui::SameLine(130);
	ImGui::DragFloat("##SpawnMaxLife", &Module.MaxLife);
	
	ImGui::Text("Spawn Shape");
	ImGui::SameLine(130);
	ImGui::DragInt("##SpawnShape", (int*)&Module.SpawnShape);
	
	ImGui::Text("Spawn Shape Scale");
	ImGui::SameLine(130);
	ImGui::DragFloat3("##SpawnShapeScale", Module.SpawnShapeScale);

	ImGui::Text("Block Spawn Shape");
	ImGui::SameLine(130);
	ImGui::DragInt("##BlockSpawnShapeScale", (int*)&Module.BlockSpawnShape);

	ImGui::Text("Block Shape Scale");
	ImGui::SameLine(130);
	ImGui::DragFloat3("##BlockSpawnShapeScale", Module.BlockSpawnShapeScale);

	ImGui::Text("Space Type");
	ImGui::SameLine(130);
	const char* szItems[] = { "Local", "World" };
	UINT Type = Module.SpaceType;
	const char* combo_preview_items = szItems[(UINT)Type];

	if (ImGui::BeginCombo("##SpaceTypeCombo", combo_preview_items))
	{
		for (int i = 0; i < 2; ++i)
		{
			const bool is_selected = ((UINT)Type == i);

			if (ImGui::Selectable(szItems[i], is_selected))
			{
				Type = i;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	Module.SpaceType = Type;

	ImGui::Separator();
	// Spawn Burst
	bool SpawnBurstModule = Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST];
	ImGui::Text("Spawn Burst Module");
	ImGui::SameLine(130);
	if (ImGui::Checkbox("##Spawn Burst Module", &SpawnBurstModule))
	{
		Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = SpawnBurstModule;
	}

	bool SpawnBurstRepeat = Module.SpawnBurstRepeat;
	ImGui::Text("Burst Repeat");
	ImGui::SameLine(130);
	if (ImGui::Checkbox("##Spawn Burst Repeat", &SpawnBurstRepeat))
	{
		Module.SpawnBurstRepeat = SpawnBurstRepeat;
	}

	ImGui::Text("Burst Count");
	ImGui::SameLine(130);
	ImGui::DragInt("##SpawnBurstCount", (int*)&Module.SpawnBurstCount);
	
	ImGui::Text("Repeat Time");
	ImGui::SameLine(130);
	ImGui::DragFloat("##SpawnBurstRepeatTime", &Module.SpawnBurstRepeatTime);

	ImGui::Separator();

	// Add Velocity
	ImGui::Text("Velocity Module");
	ImGui::SameLine(130);
	bool AddVelocityModule = Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY];
	if (ImGui::Checkbox("##AddVelocity Module", &AddVelocityModule))
	{
		Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = AddVelocityModule;
	}

	ImGui::Text("Velocity Type");
	ImGui::SameLine(130);
	const char* szVelocityItems[] = { "Random", "From Center", "To Center", "Fixed" };
	UINT VelocityType = Module.AddVelocityType;
	const char* combo_velocity_items = szVelocityItems[(UINT)VelocityType];
	if (ImGui::BeginCombo("##VelocityTypeCombo", combo_velocity_items))
	{
		for (int i = 0; i < 4; ++i)
		{
			const bool is_selected = ((UINT)VelocityType == i);

			if (ImGui::Selectable(szVelocityItems[i], is_selected))
			{
				VelocityType = i;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	Module.AddVelocityType = VelocityType;

	ImGui::Text("Fixed Dir");
	ImGui::SameLine(130);
	ImGui::DragFloat3("##AddVelocityFixedDir", Module.AddVelocityFixedDir);

	ImGui::Text("Min Speed");
	ImGui::SameLine(130);
	ImGui::DragFloat("##AddMinSpeed", &Module.AddMinSpeed);

	ImGui::Text("Max Speed");
	ImGui::SameLine(130);
	ImGui::DragFloat("##AddMaxSpeed", &Module.AddMaxSpeed);

	ImGui::Separator();

	// Scale Module
	ImGui::Text("Scale Module");
	ImGui::SameLine(130);
	
	bool ScaleModule = Module.Module[(UINT)PARTICLE_MODULE::SCALE];

	if (ImGui::Checkbox("##ScaleModule", &ScaleModule))
	{
		Module.Module[(UINT)PARTICLE_MODULE::SCALE] = ScaleModule;
	}

	ImGui::Text("Start Scale");
	ImGui::SameLine(130);
	ImGui::DragFloat("##StartScale", &Module.StartScale);
	
	ImGui::Text("End Scale");
	ImGui::SameLine(130);
	ImGui::DragFloat("##End Scale", &Module.EndScale);

	ImGui::Separator();

	// Drag Module
	ImGui::Text("Drag Module");
	ImGui::SameLine(130);
	bool DragModule = Module.Module[(UINT)PARTICLE_MODULE::DRAG];

	if (ImGui::Checkbox("##DragModule", &DragModule))
	{
		Module.Module[(UINT)PARTICLE_MODULE::DRAG] = DragModule;
	}
	
	ImGui::Text("Normalized Age");
	ImGui::SameLine(130);
	ImGui::DragFloat("##DestNormalizedAge", &Module.DestNormalizedAge);
	
	ImGui::Text("Limit Speed");
	ImGui::SameLine(130);
	ImGui::DragFloat("##LimitSpeed", &Module.LimitSpeed);
	
	ImGui::Separator();

	// Noise Force Module
	ImGui::Text("Noise Force Module");
	ImGui::SameLine(130);
	
	bool NoiseForceModule = Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE];

	if (ImGui::Checkbox("##NoiseForceModule", &NoiseForceModule))
	{
		Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = NoiseForceModule;
	}

	ImGui::Text("Term");
	ImGui::SameLine(130);
	ImGui::DragFloat("##NoiseForceTerm", &Module.NoiseForceTerm);

	ImGui::Text("Force Scale");
	ImGui::SameLine(130);
	ImGui::DragFloat("##NoiseForceScale", &Module.NoiseForceScale);

	ImGui::Separator();
	// Render Module
	ImGui::Text("Render Module");
	ImGui::SameLine(130);
	bool RenderModule = Module.Module[(UINT)PARTICLE_MODULE::RENDER];

	if (ImGui::Checkbox("##RenderModule", &RenderModule))
	{
		Module.Module[(UINT)PARTICLE_MODULE::RENDER] = RenderModule;
	}
	
	ImGui::Text("Fade Out");
	ImGui::SameLine(130);
	const char* szFadeOutItems[] = { "Off", "Normalized Age" };
	UINT FadeOutType = Module.FadeOut;
	const char* combo_FadeOut_Items = szFadeOutItems[(UINT)FadeOutType];

	if (ImGui::BeginCombo("##FadeOutTypeCombo", combo_FadeOut_Items))
	{
		for (int i = 0; i < 2; ++i)
		{
			const bool is_selected = ((UINT)FadeOutType == i);

			if (ImGui::Selectable(szFadeOutItems[i], is_selected))
			{
				FadeOutType = i;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	Module.FadeOut = FadeOutType;

	ImGui::Text("End Color");
	ImGui::SameLine(130);
	ImGui::ColorEdit3("##EndColor", Module.EndColor);

	ImGui::Text("FadeOut Ratio");
	ImGui::SameLine(130);
	ImGui::DragFloat("##FadeOutStartRatio", &Module.FadeOutStartRatio, 0.1f); // FadeOut 효과가 시작되는 Normalized Age 지점

	ImGui::Text("Alignement");
	ImGui::SameLine(130);
	const char* szAlignementItems[] = { "Off", "On" };
	UINT AlignementType = Module.VelocityAlignment;;
	const char* combo_Alignement_Items = szFadeOutItems[(UINT)AlignementType];

	if (ImGui::BeginCombo("##AlignementTypeCombo", combo_Alignement_Items))
	{
		for (int i = 0; i < 2; ++i)
		{
			const bool is_selected = ((UINT)AlignementType == i);

			if (ImGui::Selectable(szAlignementItems[i], is_selected))
			{
				AlignementType = i;
			}

			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	
	Module.VelocityAlignment = AlignementType;	// 속도 정렬 0 : Off, 1 : On

	SetChildSize(ImVec2(0.f, 1000.f));

	pParticleSys->CreateModuleBuffer();
}