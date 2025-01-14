#include "pch.h"
#include "CBoundingBox.h"

#include "CTransform.h"

CBoundingBox::CBoundingBox()
	: CComponent(COMPONENT_TYPE::BOUNDINGBOX)
	, m_Offset(1.f)
	, m_Radius(0.f)
	, m_DebugRender(false)
{
}

CBoundingBox::~CBoundingBox()
{
}

void CBoundingBox::FinalTick()
{
	Vec3 WorldScale = Transform()->GetWorldScale();

	m_Radius = WorldScale.x * m_Offset;

#ifdef _DEBUG
	if(m_DebugRender)
		DrawDebugSphere(Transform()->GetWorldPos(), m_Radius, Vec4(0.f, 1.f, 0.f, 1.f), 0.f, false);
#endif
}

void CBoundingBox::SaveToFile(FILE* _pFile)
{
	fwrite(&m_Radius, sizeof(float), 1, _pFile);
	fwrite(&m_Offset, sizeof(float), 1, _pFile);
}

void CBoundingBox::LoadFromFile(FILE* _pFile)
{
	fread(&m_Radius, sizeof(float), 1, _pFile);
	fread(&m_Offset, sizeof(float), 1, _pFile);
}