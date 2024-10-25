#include "pch.h"
#include "CLight3D.h"

#include "CAssetMgr.h"
#include "CRenderMgr.h"
#include "CTransform.h"

CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_LightIdx(-1)
{
	SetLightType(LIGHT_TYPE::DIRECTIONAL);
}

CLight3D::CLight3D(const CLight3D& _Origin)
	: CComponent(_Origin)
	, m_LightIdx(-1)
	, m_Info(_Origin.m_Info)
{
	SetLightType(m_Info.Type);
}

CLight3D::~CLight3D()
{
}

void CLight3D::FinalTick()
{
	m_Info.WorldPos = Transform()->GetWorldPos();
	m_Info.WorldDir = Transform()->GetWorldDir(DIR::FRONT);

	if (LIGHT_TYPE::POINT == m_Info.Type)
		Transform()->SetRelativeScale(m_Info.Radius * 2.f, m_Info.Radius * 2.f, m_Info.Radius * 2.f);
	else if(LIGHT_TYPE::SPOT == m_Info.Type)
		Transform()->SetRelativeScale(m_Info.Radius, m_Info.Radius, m_Info.Radius);

	// 자신을 RenderMgr 에 등록시킴
	m_LightIdx = CRenderMgr::GetInst()->RegisterLight3D(this);

#ifdef _DEBUG
	if (LIGHT_TYPE::POINT == m_Info.Type)
		DrawDebugSphere(Transform()->GetWorldMatrix(), Vec4(0.f, 1.f, 0.f, 1.f), 0.f, true);
	else if(LIGHT_TYPE::SPOT == m_Info.Type)
		DrawDebugCone(Transform()->GetWorldMatrix(), Vec4(0.f, 1.f, 0.f, 1.f), 0.f, true);
#endif
}

void CLight3D::Render()
{
	Transform()->Binding();

	m_LightMtrl->SetScalarParam(INT_0, m_LightIdx);
	m_LightMtrl->Binding();

	m_VolumeMesh->Render();
}

void CLight3D::SetLightType(LIGHT_TYPE _Type)
{
	m_Info.Type = _Type;

	if (LIGHT_TYPE::DIRECTIONAL == _Type)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
		m_LightMtrl  = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DirLightMtrl");
	}
	else if (LIGHT_TYPE::POINT == _Type)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh");
		m_LightMtrl  = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PointLightMtrl");
	}
	else if (LIGHT_TYPE::SPOT == _Type)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"ConeMesh");
		m_LightMtrl  = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpotLightMtrl");
	}
}

void CLight3D::SaveToFile(FILE* _pFile)
{
	fwrite(&m_Info, sizeof(tLightInfo), 1, _pFile);
	SaveAssetRef(m_VolumeMesh, _pFile);
	SaveAssetRef(m_LightMtrl, _pFile);
}

void CLight3D::LoadFromFile(FILE* _pFile)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _pFile);
	LoadAssetRef(m_VolumeMesh, _pFile);
	LoadAssetRef(m_LightMtrl, _pFile);
}