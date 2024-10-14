#include "pch.h"
#include "CLight2D.h"

#include "CRenderMgr.h"
#include "CTransform.h"

CLight2D::CLight2D()
	: CComponent(COMPONENT_TYPE::LIGHT2D)
	, m_Info{}
{
}

CLight2D::~CLight2D()
{
}

void CLight2D::FinalTick()
{
	m_Info.WorldPos = Transform()->GetWorldPos();
	m_Info.WorldDir = Transform()->GetWorldDir(DIR::RIGHT);

	// RenderMgr ¿¡ µî·Ï
	CRenderMgr::GetInst()->RegisterLight2D(this);
}

void CLight2D::SetLightType(LIGHT_TYPE _Type)
{
	m_Info.Type = _Type;
}

void CLight2D::SaveToFile(FILE* _pFile)
{
	fwrite(&m_Info, sizeof(tLightInfo), 1, _pFile);
}

void CLight2D::LoadFromFile(FILE* _pFile)
{
	fread(&m_Info, sizeof(tLightInfo), 1, _pFile);
}