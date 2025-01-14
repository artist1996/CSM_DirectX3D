#include "pch.h"
#include "CCollider3D.h"

#include "CKeyMgr.h"

#include "CGameObject.h"
#include "CTransform.h"

#include "CScript.h"

CCollider3D::CCollider3D()
	: CComponent(COMPONENT_TYPE::COLLIDER3D)
	, m_OverlapCount(0)
	, m_IndependentScale(false)
	, m_bRender(true)
	, m_bActive(false)
{
}

CCollider3D::CCollider3D(const CCollider3D& _Origin)
	: CComponent(_Origin)
	, m_Offset(_Origin.m_Offset)
	, m_Scale(_Origin.m_Scale)
	, m_OverlapCount(0)
	, m_IndependentScale(_Origin.m_IndependentScale)
	, m_bRender(_Origin.m_bRender)
	, m_bActive(_Origin.m_bActive)
{
}

CCollider3D::~CCollider3D()
{
}

void CCollider3D::FinalTick()
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

	m_matColWorld = matScale * matTranslation * matObjectScaleInv * GetOwner()->Transform()->GetWorldMat();

	if (m_bRender)
	{
		if (m_OverlapCount)
			DrawDebugCube(m_matColWorld, Vec4(1.f, 0.f, 0.f, 1.f), 0.f, false);
		else
			DrawDebugCube(m_matColWorld, Vec4(0.f, 1.f, 0.f, 1.f), 0.f, false);
	}
}

void CCollider3D::SaveToFile(FILE* _pFile)
{
}

void CCollider3D::LoadFromFile(FILE* _pFile)
{
}

void CCollider3D::BeginOverlap(CCollider3D* _OtherCollider)
{
	const vector<CScript*> vecScripts = GetOwner()->GetScripts();

	m_OverlapCount += 1;

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->BeginOverlap3D(this, _OtherCollider->GetOwner(), _OtherCollider);
	}
}

void CCollider3D::Overlap(CCollider3D* _OtherCollider)
{
	const vector<CScript*> vecScripts = GetOwner()->GetScripts();

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->Overlap3D(this, _OtherCollider->GetOwner(), _OtherCollider);
	}
}

void CCollider3D::EndOverlap(CCollider3D* _OtherCollider)
{
	const vector<CScript*> vecScripts = GetOwner()->GetScripts();

	m_OverlapCount -= 1;

	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->EndOverlap3D(this, _OtherCollider->GetOwner(), _OtherCollider);
	}
}