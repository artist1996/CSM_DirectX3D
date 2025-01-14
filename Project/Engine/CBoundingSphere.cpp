#include "pch.h"
#include "CBoundingSphere.h"

#include "CKeyMgr.h"
#include "CTransform.h"

CBoundingSphere::CBoundingSphere()
	: CComponent(COMPONENT_TYPE::BOUNDINGSPHERE)
	, m_Offset(1.f)
	, m_Radius(0.f)
	, m_DebugRender(true)
{
}

CBoundingSphere::~CBoundingSphere()
{
}

void CBoundingSphere::FinalTick()
{
	if (KEY_TAP(KEY::_0))
		m_DebugRender = !m_DebugRender;

	Vec3 WorldScale = Transform()->GetWorldScale();

	m_Radius = (WorldScale.x * m_Offset) * 0.5f;

#ifdef _DEBUG
	if (m_DebugRender)
		DrawDebugSphere(Transform()->GetWorldPos(), m_Radius * 2.f, Vec4(0.f, 1.f, 0.f, 1.f), 0.f, false);
#endif
}

void CBoundingSphere::SaveToFile(FILE* _pFile)
{
	fwrite(&m_Radius, sizeof(float), 1, _pFile);
	fwrite(&m_Offset, sizeof(float), 1, _pFile);
}

void CBoundingSphere::LoadFromFile(FILE* _pFile)
{
	fread(&m_Radius, sizeof(float), 1, _pFile);
	fread(&m_Offset, sizeof(float), 1, _pFile);
}