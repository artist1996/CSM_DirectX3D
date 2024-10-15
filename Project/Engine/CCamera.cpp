#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"
#include "CRenderMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CRenderComponent.h"

#include "CTransform.h"
#include "CCollider2D.h"

#include "CKeyMgr.h"
#include "CTimeMgr.h"

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_Priority(-1)
	, m_LayerCheck(0)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_Width(0.f)
	, m_Height(0.f)
	, m_Far(100000.f)
	, m_FOV(XM_PI / 2.f)
	, m_ProjectionScale(1.f)
	, m_AspectRatio(0.f)
	, m_ZoomIn(false)
	, m_ZoomOut(false)
	, m_ShakingIn(false)
	, m_ShakingOut(false)
	, m_UI(false)
{
	Vec2 vResolution = CDevice::GetInst()->GetResolution();
	//m_Width = vResolution.x;
	//m_Height = vResolution.y;
	m_Width  = 1280.f;
	m_Height = 768.f;
	m_AspectRatio = m_Width / m_Height;
}

CCamera::~CCamera()
{
}

void CCamera::Begin()
{
	// 카메라 등록
	if (-1 != m_Priority)
	{
		CRenderMgr::GetInst()->RegisterCamera(this, m_Priority);
	}
}

void CCamera::FinalTick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	// View Space 란 카메라가 좌표계의 기준이 되는 좌표계
	// 1. 카메라가 원점에 존재
	// 2. 카메라가 바라보는 방향이 Z 축
	
	// View 행렬을 계산한다.
	// View 행렬은 World Space -> View Space 로 변경할때 사용하는 행렬
	
	Matrix matTrans = XMMatrixTranslation(-Transform()->GetRelativePos().x, -Transform()->GetRelativePos().y, -Transform()->GetRelativePos().z);
	Matrix matRot;
	Vec3 vR = Transform()->GetWorldDir(DIR::RIGHT);
	Vec3 vU = Transform()->GetWorldDir(DIR::UP);
	Vec3 vF = Transform()->GetWorldDir(DIR::FRONT);

	// 전치
	matRot._11 = vR.x; matRot._12 = vU.x; matRot._13 = vF.x;
	matRot._21 = vR.y; matRot._22 = vU.y; matRot._23 = vF.y;
	matRot._31 = vR.z; matRot._32 = vU.z; matRot._33 = vF.z;

	m_matView = matTrans * matRot;
	
	// Projection Space 투영 좌표계(NDC)
	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// 1. 직교 투영 (Orthographic)
		// 투영을 일직선으로
		// 시야 범위를 NDC 로 압축
		m_matProj = XMMatrixOrthographicLH(m_Width * m_ProjectionScale, m_Height * m_ProjectionScale, 1.f, m_Far);
	}

	else
	{
		// 2. 원근 투영 (Perspective)
		// 종횡비(Ratio)
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}
}

void CCamera::SortGameObject()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (false == (m_LayerCheck & (1 << i)))
		{
			continue;
		}

		CLayer* pLayer = pLevel->GetLayer(i);

		const vector<CGameObject*>& vecObjects = pLayer->GetObjects();

				
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (nullptr == vecObjects[j]->GetRenderComponent()
			 || nullptr == vecObjects[j]->GetRenderComponent()->GetMesh()
			 || nullptr == vecObjects[j]->GetRenderComponent()->GetMaterial()
			 || nullptr == vecObjects[j]->GetRenderComponent()->GetMaterial()->GetShader())
			{
				continue;
			}
			
			Ptr<CGraphicShader> pShader = vecObjects[j]->GetRenderComponent()->GetMaterial()->GetShader();
			SHADER_DOMAIN Domain = pShader->GetDomain();
			
			switch (Domain)
			{
			case DOMAIN_OPAQUE:
				m_vecOpaque.push_back(vecObjects[j]);
				break;
			case DOMAIN_MASKED:
				m_vecMasked.push_back(vecObjects[j]);
				//YSorting();
				break;
			case DOMAIN_TRANSPARENT:
				m_vecTransparent.push_back(vecObjects[j]);
				break;
			case DOMAIN_PARTICLE:
				m_vecParticles.push_back(vecObjects[j]);
				break;
			case DOMAIN_POSTPROCESS:
				m_vecPostProcess.push_back(vecObjects[j]);
				break;
			case DOMAIN_UI:
				m_vecUI.push_back(vecObjects[j]);
				break;
			}
		}
	}
}

void CCamera::Render()
{
	SortGameObject();

	g_Trans.matView = m_matView;
	g_Trans.matProj = m_matProj;

	// Opaque
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->Render();
	}

	// Masked
	for (size_t i = 0; i < m_vecMasked.size(); ++i)
	{
		m_vecMasked[i]->Render();
	}

	// Transparent
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
		m_vecTransparent[i]->Render();
	}

	// Particles
	for (size_t i = 0; i < m_vecParticles.size(); ++i)
	{
		m_vecParticles[i]->Render();
	}

	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		CRenderMgr::GetInst()->PostProcessCopy();
		m_vecPostProcess[i]->Render();
	}

	for (size_t i = 0; i < m_vecUI.size(); ++i)
	{
		m_vecUI[i]->Render();
	}

	// Runtime 중 Domain Type이 변경 될 수 있기 때문에 Render 호출 하고 clear 시켜줌
	m_vecOpaque.clear();
	m_vecMasked.clear();
	m_vecTransparent.clear();
	m_vecParticles.clear();
	m_vecPostProcess.clear();
	m_vecUI.clear();
}

void CCamera::SaveToFile(FILE* _pFile)
{
	fwrite(&m_Priority, sizeof(int), 1, _pFile);
	fwrite(&m_LayerCheck, sizeof(UINT), 1, _pFile);
	fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _pFile);
	fwrite(&m_Width, sizeof(float), 1, _pFile);
	fwrite(&m_Height, sizeof(float), 1, _pFile);
	fwrite(&m_Far, sizeof(float), 1, _pFile);
	fwrite(&m_AspectRatio, sizeof(float), 1, _pFile);
	fwrite(&m_FOV, sizeof(float), 1, _pFile);
	fwrite(&m_ProjectionScale, sizeof(float), 1, _pFile);
}

void CCamera::LoadFromFile(FILE* _pFile)
{
	fread(&m_Priority, sizeof(int), 1, _pFile);
	fread(&m_LayerCheck, sizeof(UINT), 1, _pFile);
	fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _pFile);
	fread(&m_Width, sizeof(float), 1, _pFile);
	fread(&m_Height, sizeof(float), 1, _pFile);
	fread(&m_Far, sizeof(float), 1, _pFile);
	fread(&m_AspectRatio, sizeof(float), 1, _pFile);
	fread(&m_FOV, sizeof(float), 1, _pFile);
	fread(&m_ProjectionScale, sizeof(float), 1, _pFile);
}