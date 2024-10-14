#include "pch.h"
#include "CCollider2D.h"

#include "CKeyMgr.h"
#include "CTransform.h"
#include "CScript.h"

CCollider2D::CCollider2D()
	: CComponent(COMPONENT_TYPE::COLLIDER2D)
	, m_OverlapCount(0)
	, m_IndependentScale(false)
	, m_bRender(false)
	, m_bActive(false)
{
}

CCollider2D::CCollider2D(const CCollider2D& _Origin)
	: CComponent(_Origin)
	, m_Offset(_Origin.m_Offset)
	, m_Scale(_Origin.m_Scale)
	, m_OverlapCount(0)
	, m_IndependentScale(_Origin.m_IndependentScale)
	, m_bRender(_Origin.m_bRender)
	, m_bActive(_Origin.m_bActive)
{
}

CCollider2D::~CCollider2D()
{
}

void CCollider2D::FinalTick()
{	
	if (KEY_TAP(KEY::_0))
		m_bRender = !m_bRender;

	// Offset 행렬 구하기
	Matrix matTranslation = XMMatrixTranslation(m_Offset.x, m_Offset.y, m_Offset.z);
	
	// Scale 구하기
	Matrix matScale = XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);

	Matrix matObjectScaleInv = XMMatrixIdentity();

	// 독립적인 크기를 갖는다면
	if (m_IndependentScale)
	{
		// 크기 행렬의 역행렬을 곱 해준다.
		Vec3 vObjectScale = GetOwner()->Transform()->GetWorldScale();
		matObjectScaleInv = XMMatrixScaling(vObjectScale.x, vObjectScale.y, vObjectScale.z);
		matObjectScaleInv = XMMatrixInverse(nullptr, matObjectScaleInv);
	}

	m_matColWorld = matScale * matTranslation * matObjectScaleInv * GetOwner()->Transform()->GetWorldMatrix();

	
	if (m_bRender)
	{
		if (m_OverlapCount)
			DrawDebugRect(m_matColWorld, Vec4(1.f, 0.f, 0.f, 1.f), 0.f, false);
		else
			DrawDebugRect(m_matColWorld, Vec4(0.f, 1.f, 0.f, 1.f), 0.f, false);
	}
}

void CCollider2D::BeginOverlap(CCollider2D* _OtherCollider)
{
	const vector<CScript*> vecScripts = GetOwner()->GetScripts();

	m_OverlapCount += 1;

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->BeginOverlap(this, _OtherCollider->GetOwner(), _OtherCollider);
	}
}

void CCollider2D::Overlap(CCollider2D* _OtherCollider)
{
	const vector<CScript*> vecScripts = GetOwner()->GetScripts();

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->Overlap(this, _OtherCollider->GetOwner(), _OtherCollider);
	}
}

void CCollider2D::EndOverlap(CCollider2D* _OtherCollider)
{
	const vector<CScript*> vecScripts = GetOwner()->GetScripts();

	m_OverlapCount -= 1;

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->EndOverlap(this, _OtherCollider->GetOwner(), _OtherCollider);
	}
}

void CCollider2D::SaveToFile(FILE* _pFile)
{
	fwrite(&m_Offset, sizeof(Vec3), 1, _pFile);
	fwrite(&m_Scale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_IndependentScale, sizeof(bool), 1, _pFile);
}

void CCollider2D::LoadFromFile(FILE* _pFile)
{
	fread(&m_Offset, sizeof(Vec3), 1, _pFile);
	fread(&m_Scale, sizeof(Vec3), 1, _pFile);
	fread(&m_IndependentScale, sizeof(bool), 1, _pFile);
}