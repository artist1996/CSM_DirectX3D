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

CLight3D::~CLight3D()
{
}

void CLight3D::FinalTick()
{
	m_Info.WorldPos = Transform()->GetWorldPos();
	m_Info.WorldDir = Transform()->GetWorldDir(DIR::FRONT);

	// 자신을 RenderMgr 에 등록시킴
	m_LightIdx = CRenderMgr::GetInst()->RegisterLight3D(this);
}

void CLight3D::Render()
{
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
		m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DirLightMtrl");
	}
	else if (LIGHT_TYPE::POINT == _Type)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"SphereMesh");
		m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"PointLightMtrl");
	}
	else if (LIGHT_TYPE::SPOT == _Type)
	{
		m_VolumeMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"ConeMesh");
		m_LightMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"SpotLightMtrl");
	}
}

void CLight3D::SaveToFile(FILE* _pFile)
{
	fwrite(&m_Info, sizeof(tLightInfo), 1, _pFile);
}

void CLight3D::LoadFromFile(FILE* _pFile)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _pFile);
}