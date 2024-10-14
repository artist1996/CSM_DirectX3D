#include "pch.h"
#include "CParticleSystem.h"

#include "CTimeMgr.h"
#include "CDevice.h"
#include "CAssetMgr.h"
#include "CStructuredBuffer.h"

#include "CTransform.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_TickCS(nullptr)
	, m_ParticleTex(nullptr)
	, m_Module{}
	, m_Time(0.f)
	, m_BurstTime(0.f)
	, m_MaxParticleCount(1000)
{
	// Mesh Material
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleRenderMtrl"));
	
	// ParticleTick ComputeShader
	m_TickCS = (CParticleTickCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleTickCS").Get();
	
	// Particle Texture
	m_ParticleTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\particle\\FX_Flare.png");
	m_EndTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\particle\\Ripple.png");
	
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::SRV_UAV, true, nullptr);
	
	m_SpawnCountBuffer = new CStructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::SRV_UAV, true, nullptr);
	
	// 파티클 기능(모듈) 정보 세팅
	// Spawn Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
	m_Module.SpawnRate			    = 30;
	m_Module.vSpawnColor		    = Vec4(0.24f, 0.67f, 0.87f, 1.f);
	m_Module.MinLife			    = 1.f;
	m_Module.MaxLife			    = 5.f;
	m_Module.vSpawnMinScale		    = Vec3(5.f, 5.f, 1.f);
	m_Module.vSpawnMaxScale		    = Vec3(50.f, 50.f, 1.f);
								    
	m_Module.SpawnShape			    = 1;
	m_Module.SpawnShapeScale.x	    = 500.f;

	m_Module.BlockSpawnShape        = 1;
	m_Module.BlockSpawnShapeScale.x = 0.f;

	m_Module.SpaceType				= 0;	// Local Space

	// Spawn Burst Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
	m_Module.SpawnBurstRepeat	  = true;
	m_Module.SpawnBurstCount	  = 500;
	m_Module.SpawnBurstRepeatTime = 3.f;

	// Scale Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
	m_Module.StartScale = 1.f;
	m_Module.EndScale   = 1.f;
	
	// AddVelocity Module
	m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	m_Module.AddVelocityType	 = 1;	// 0 : Random, 1 : From Center, 2: To Center, 3 : Fixed
	m_Module.AddVelocityFixedDir = Vec3(0.f, -1.f, 1.f);
	m_Module.AddMinSpeed		 = 100.f;
	m_Module.AddMaxSpeed		 = 300.f;

	// Drag Module (감속)
	m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = false;
	m_Module.DestNormalizedAge = 1.f;
	m_Module.LimitSpeed		   = 100.f;

	// Noise Force Module
	m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	m_Module.NoiseForceTerm  = 0.3f;
	m_Module.NoiseForceScale = 100.f;

	// Render Module
	m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
	m_Module.EndColor = Vec3(1.f, 0.f, 0.f);
	m_Module.FadeOut = true;
	m_Module.FadeOutStartRatio = 0.9f;
	m_Module.VelocityAlignment = true;

	m_ModuleBuffer = new CStructuredBuffer;
	m_ModuleBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::SRV_UAV, true, &m_Module);
}

CParticleSystem::CParticleSystem(const CParticleSystem& _Origin)
	: CRenderComponent(_Origin)
	, m_TickCS(_Origin.m_TickCS)
	, m_ParticleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_ModuleBuffer(nullptr)
	, m_ParticleTex(_Origin.m_ParticleTex)
	, m_EndTex(_Origin.m_EndTex)
	, m_Time(0.f)
	, m_BurstTime(0.f)
	, m_MaxParticleCount(_Origin.m_MaxParticleCount)
	, m_Module(_Origin.m_Module)
{
	assert(_Origin.m_ParticleBuffer && _Origin.m_SpawnCountBuffer && _Origin.m_ModuleBuffer);
	
	m_ParticleBuffer   = new CStructuredBuffer(*_Origin.m_ParticleBuffer);
	m_SpawnCountBuffer = new CStructuredBuffer(*_Origin.m_SpawnCountBuffer);
	m_ModuleBuffer	   = new CStructuredBuffer(*_Origin.m_ModuleBuffer);
}


CParticleSystem::~CParticleSystem()
{
	SAFE_DELETE(m_ParticleBuffer);
	SAFE_DELETE(m_SpawnCountBuffer);
	SAFE_DELETE(m_ModuleBuffer);
}

void CParticleSystem::CreateModuleBuffer()
{
	m_ModuleBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::SRV_UAV, true, &m_Module);
}

void CParticleSystem::FinalTick()
{
	CarculateSpawnCount();
	CreateModuleBuffer();
	// Compute Shader
	m_TickCS->SetParticleWorldPos(Transform()->GetWorldPos());
	m_TickCS->SetParticleBuffer(m_ParticleBuffer);
	m_TickCS->SetSpawnCountBuffer(m_SpawnCountBuffer);
	m_TickCS->SetModuleBuffer(m_ModuleBuffer);
	m_TickCS->Execute();
}

void CParticleSystem::Render()
{
	// Transform Binding
	Transform()->Binding();

	// Particle Buffer Binding
	m_ParticleBuffer->Binding(20);	 // t20

	// Module Buffer Binding
	m_ModuleBuffer->Binding(21);	 // t21
	// Material Binding

	GetMaterial()->SetTexParam(TEX_0, m_ParticleTex);
	GetMaterial()->SetTexParam(TEX_1, m_EndTex);

	GetMaterial()->Binding();

	// Rendering
	GetMesh()->Render_Particle(m_MaxParticleCount);

	m_ParticleBuffer->Clear(20);
	m_ModuleBuffer->Clear(21);
}

void CParticleSystem::CarculateSpawnCount()
{
	m_Time += EngineDT;

	tSpawnCount Count = {};

	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN])
	{
		// SpawnRate 에 따른 이번 Tick 에서 생성시킬 파티클의 수
		float Term = 1.f / (float)m_Module.SpawnRate;
		UINT SpawnCount = 0;

		if (Term < m_Time)
		{
			float Value = m_Time / Term;
			SpawnCount  = (UINT)Value;
			m_Time     -= (float)SpawnCount * Term;
		}

		Count.iSpawnCount = SpawnCount;
	}

	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST])
	{
		UINT BurstCount = 0;
		if (0.f == m_BurstTime)
		{
			BurstCount = m_Module.SpawnBurstCount;
		}

		m_BurstTime += EngineDT;

		if (m_Module.SpawnBurstRepeat && m_Module.SpawnBurstRepeatTime <= m_BurstTime)
		{
			m_BurstTime -= m_Module.SpawnBurstRepeatTime;
			BurstCount += m_Module.SpawnBurstCount;
		}

		Count.iSpawnCount += BurstCount;
	}

	// SpawnCount 를 Buffer에 전달
	m_SpawnCountBuffer->SetData(&Count);
}

void CParticleSystem::SaveToFile(FILE* _File)
{
	SaveAssetRef(m_ParticleTex, _File);

	fwrite(&m_MaxParticleCount, sizeof(int), 1, _File);
	fwrite(&m_Module, sizeof(tParticleModule), 1, _File);
}

void CParticleSystem::LoadFromFile(FILE* _File)
{
	LoadAssetRef(m_ParticleTex, _File);

	fread(&m_MaxParticleCount, sizeof(int), 1, _File);
	fread(&m_Module, sizeof(tParticleModule), 1, _File);
}
